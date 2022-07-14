/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:20:40 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/14 02:46:34 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_listoar(t_shell *shell)
{
	int		i;
	int		size;
	char	**env_array;
	t_env	*tmp;

	tmp = shell->senv;
	if (tmp)
		tmp = tmp->next;
	size = 0;
	while (tmp && ++size)
		tmp = tmp->next;
	env_array = ft_calloc(size + 1, sizeof(*env_array));
	if (!env_array)
		exit_free(shell);
	env_array[size] = NULL;
	tmp = shell->senv;
	if (tmp)
		tmp = tmp->next;
	i = 0;
	while (tmp)
	{
		env_array[i++] = tmp->str;
		tmp = tmp->next;
	}
	return (env_array);
}

char	**get_cmd_tab(t_shell *shell)
{
	int		i;
	int		len;
	t_list	*tmp;
	char	**tab;

	len = 1;
	tmp = shell->current;
	while (pop_pos(tmp) < pop_pos(tmp->next))
	{
		len++;
		tmp = tmp->next;
	}
	tab = ft_calloc(len + 1, sizeof(char *));
	if (!tab)
		exit_free(shell);
	tab[len] = NULL;
	tmp = shell->current;
	i = -1;
	while (++i < len)
	{
		tab[i] = pop_key(tmp);
		tmp = tmp->next;
	}
	return (tab);
}

int	exec_builtin(t_shell *shell, char **tab)
{
	int	ret;

	ret = 0;
	if (ft_strequ("pwd", tab[0]))
		ret = msh_pwd(shell);
	else if (ft_strequ("env", tab[0]))
		ret = msh_env(shell, tab);
	else if (ft_strequ("echo", tab[0]))
		ret = msh_echo(shell, tab);
	else if (ft_strequ("export", tab[0]))
		ret = msh_export(shell, tab);
	else if (ft_strequ("unset", tab[0]))
		ret = msh_unset(shell, tab);
	else if (ft_strequ("cd", tab[0]))
		ret = msh_cd(shell, tab);
	if (ret)
		return (2);
	return (0);
}

int	exec_bin(t_shell *shell, char **tab)
{
	pid_t	pid;
	char	**env_array;

	pid = 0;
	if (shell->parent)
		sig.pid = fork();
	if (sig.pid == -1)
		return (print_errno("fork", 1));
	if (sig.pid == 0)
	{
		env_array = env_listoar(shell);
		execve(*tab, tab, env_array);
		// ft_printf("%s\n", "child");
		print_errno(*tab, 126);
		free(env_array);
		free_shell(shell);
		exit(126);
	}
	else
	{
		waitpid(sig.pid, &shell->ret, 0);
		if (shell->ret)
			shell->ret /= 256;
		if (sig.signal)
			shell->ret = sig.signal;
		if (sig.signal == 131)
			ft_printf("Quit: 3\n");
	}
	return (shell->ret);
}

void	pipe_process(t_shell *shell, int fd[2], pid_t pid)
{
	if (pid == 0)
	{
		// ft_dprintf(shell->fd_stdout, "%sPIPE created [child]\n", GREEN);
		shell->parent = 0;
		// ft_dprintf(shell->fd_stdout, "PIPE_FDs [0] = %d, [1] = %d\n", fd[0], fd[1]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			shell->ret = print_errno("dup2", 1);
		close(fd[0]);
		close(fd[1]);
		// ft_dprintf(shell->fd_stdout, "DUP RET child = %d\n", ret);
	}
	else
	{
		// sleep(1);
		// ft_dprintf(shell->fd_stdout, "ret%d\n", shell->ret);
		waitpid(pid, &shell->ret, 0);
		if (shell->ret)
			shell->ret /= 256;
		// ft_dprintf(shell->fd_stdout, "%s", RESET);
		// ft_dprintf(shell->fd_stdout, "ret after pid in PIPE = %d\n", shell->ret);
		if (dup2(fd[0], STDIN_FILENO) == -1)
			shell->ret = print_errno("dup2", 1);
		close(fd[0]);
		close(fd[1]);
		// ft_dprintf(shell->fd_stdout, "DUP RET parent = %d\n", ret);
	}
}

int	open_pipe(t_shell *shell)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (print_errno("pipe", 1));
	pid = fork();
	if (pid == -1)
		return (print_errno("fork", 1));
	pipe_process(shell, fd, pid);
	return (shell->ret);
}

int	handle_pipe(t_shell *shell, t_symbol exec_type, char **tab)
{
	int	fd[2];

	if (exec_type == T_BIN)
	{
	// ft_dprintf(shell->fd_stdout, "PIPE(bin) %s\n", shell->parent? "[parent]" : "[child]");
		shell->ret = open_pipe(shell);
		free(tab);
	}
	else if (exec_type == T_BUILTIN)
	{
		// ft_dprintf(shell->fd_stdout, "PIPE(builtin) %s\n", shell->parent? "[parent]" : "[child]");
		if (pipe(fd) == -1)
			shell->ret = print_errno("pipe", 1);
		if (!shell->ret && dup2(fd[1], STDOUT_FILENO) == -1)
			shell->ret = print_errno("dup2", 1);
		close(fd[1]);
		shell->end = 1;
	}
	return (fd[0]);
}

void redir_in(t_shell *shell, t_symbol symbol)
{
	int		fdin;
	char	*path;
	t_hdoc	*tmp;

	path = pop_key(shell->current);
	if (symbol == T_REDIRIN)
	{
		// ft_dprintf(shell->fd_stdout, " REDIR IN <[ %s ] for %s\n", path, shell->parent? "[parent]" : "[child]");
		fdin = open(path, O_RDONLY, 0666);
		if (fdin == -1 || dup2(fdin, STDIN_FILENO) == -1)
			shell->ret = print_errno(path, 1);
		if (fdin != -1)
			close(fdin);
	}
	else
	{
		// ft_dprintf(shell->fd_stdout, " REDIR IN << [ %s ] for %s\n", pop_key(shell->current), shell->parent? "[parent]" : "[child]");
		if (dup2(shell->heredoc->fd, STDIN_FILENO) == -1)
			shell->ret = print_errno("heredoc", 1);
		close(shell->heredoc->fd);
		tmp = shell->heredoc;
		shell->heredoc = shell->heredoc->next;
		free(tmp);
	}
	// return (shell->ret);
}

void redir_out(t_shell *shell, t_symbol symbol)
{
	int		fdout;
	char	*path;

	path = pop_key(shell->current);
	if (symbol == T_REDIROUT)
	{
		// ft_dprintf(shell->fd_stdout, " REDIR OUT > [ %s ] for %s\n", path, shell->parent? "[parent]" : "[child]");
		fdout = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fdout == -1)
			shell->ret = print_errno(path, 1);
	}
	else
	{
		// ft_dprintf(shell->fd_stdout, " APPEND >> [ %s ] for %s\n", path, shell->parent? "[parent]" : "[child]");
		fdout = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fdout == -1)
			shell->ret = print_errno(path, 1);
		// ft_dprintf(shell->fd_stdout, "fd = %d\n", fdout);
	}
	if (!shell->ret && dup2(fdout, STDOUT_FILENO) == -1)
		shell->ret = print_errno(path, 1);
	if (fdout != -1)
		close(fdout);
	// return (0);
}

void	redirect(t_shell *shell, t_symbol symbol)
{
	// int ret;

	// ret = 0;
	// ft_dprintf(shell->fd_stdout, "errno = %d, shellret = %d before redir for %s\n", errno, shell->ret, shell->parent? "[parent]" : "[child]");
	if (symbol == T_REDIRIN || symbol == T_HEREDOC)
		redir_in(shell, symbol);
	else
		redir_out(shell, symbol);
	// ft_dprintf(shell->fd_stdout, "errno = %d, shellret = %d after redir for %s\n", errno, shell->ret, shell->parent? "[parent]" : "[child]");
	// return (ret);
}

void	handle_redir(t_shell *shell, t_symbol symbol, char **tab)
{
	t_hdoc		*tmp;

	if (!shell->parent || shell->end)
	{
		redirect(shell, symbol);
		if (shell->ret)
			free(tab);
	}
	else if (symbol == T_HEREDOC)
	{
		tmp = shell->heredoc;
		close(tmp->fd);
		shell->heredoc = shell->heredoc->next;
		free(tmp);
	}
}

int	is_selected_for_execution(t_shell *shell, char **tab)
{
	return ((!shell->current || pop_pos(shell->current) == 0) \
			&& (!shell->parent || shell->end) \
			&& !shell->ret \
			&& tab);
}

void	execute(t_shell *shell, t_symbol exec_type, char **tab, int fdin_pipe)
{
	if (exec_type == T_BIN)
		shell->ret = exec_bin(shell, tab);
	else
	{
		shell->ret = exec_builtin(shell, tab);
		// ft_dprintf(shell->fd_stdout, "%s who's executed builtin now listen from [%d]\n", shell->parent? "[parent]" : "[child]", fd[0]);
		if (dup2(fdin_pipe, STDIN_FILENO) == -1)
			shell->ret = print_errno("1dup2", 1);
		if (dup2(shell->fd_stdout, STDOUT_FILENO) == -1)
			shell->ret = print_errno("2dup2", 1);
		close(fdin_pipe);
		shell->end = 0;
	}
	free(tab);
	tab = NULL;
}


int	executor(t_shell *shell, char **tab)
{
	int			fdin_pipe;
	t_symbol	symbol;
	t_symbol	exec_type;

	shell->current = shell->start;
	fdin_pipe = 0;
	while (shell->current && !shell->ret)
	{
		symbol = pop_symbol(shell->current);
		if (symbol == T_NEWLINE)
			shell->end = 1;
		else if (symbol == T_PIPE)
			fdin_pipe = handle_pipe(shell, exec_type, tab);
		else if (is_redir(symbol))
			handle_redir(shell, symbol, tab);
		else if (!pop_pos(shell->current) && (!shell->parent || !shell->end))
		{
			tab = get_cmd_tab(shell);
			exec_type = pop_symbol(shell->current);
		}
		shell->current = shell->current->next;
		if (is_selected_for_execution(shell, tab))
			execute(shell, exec_type, tab, fdin_pipe);
	}
	return (shell->ret);
}
