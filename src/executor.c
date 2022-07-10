/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:20:40 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/10 16:15:23 by jcarere          ###   ########.fr       */
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
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	env_array = ft_calloc(size + 1, sizeof(*env_array));
	if (!env_array)
		exit_free(shell);
	env_array[size] = NULL;
	tmp = shell->senv;
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
	if (ft_strequ("pwd", tab[0]))
		msh_pwd(shell);
	else if (ft_strequ("env", tab[0]))
		msh_env(shell, tab);
	else if (ft_strequ("echo", tab[0]))
		msh_echo(shell, tab);
	else if (ft_strequ("export", tab[0]))
		msh_export(shell, tab);
	else if (ft_strequ("unset", tab[0]))
		msh_unset(shell, tab);
	else if (ft_strequ("cd", tab[0]))
		msh_cd(shell, tab);
	else
		ft_printf("%s\n", "exec other builtin");
	return (0);
}

int	exec_bin(t_shell *shell, char **tab)
{
	int i;

	i = -1;
	// ft_dprintf(shell->fd_stdout, "EXECUTE [");
	// while (tab[++i])
		// ft_dprintf(shell->fd_stdout, " %s", tab[i]);
	// ft_dprintf(shell->fd_stdout, " ] for %s\n", shell->parent?"[parent]":"[piped child]");
	pid_t	pid;
	char	**env_array;

	pid = 0;
	if (shell->parent)
	{
		// ft_dprintf(shell->fd_stdout, "%sEXECUTE created [child]\n", GREEN);
		pid = fork();
	}
	if (pid < 0)
		return (errno);
	if (pid == 0)
	{
		// ft_dprintf(shell->fd_stdout, "%s execute child\n", RED);
		env_array = env_listoar(shell);
		// ft_dprintf(shell->fd_stdout, "cmd = %s, first arg = %s\n", *tab, *(tab + 1));
		execve(*tab, tab, env_array);
		// ft_dprintf(shell->fd_stdout, "exec failed %d\n", shell->ret);
		// print_error(shell);
		// exit(0);
		// shell->ret = errno;
		free(env_array);
		free_shell(shell);
		exit(0);
		// free(tab);
		// exit_free(shell);
	}
	else
	{
		waitpid(pid, NULL, 0);
		// ft_dprintf(shell->fd_stdout, "%s[parent] finish waiting\n", RESET);
	}
	// ft_dprintf(shell->fd_stdout, "free tab ptr in %s %p\n",shell->parent? "[parent]" : "[child]", tab);
	free(tab);
	// waitpid(-1, &shell->ret, 0);
	// shell->ret = WEXITSTATUS(shell->ret);
	// ft_dprintf(shell->fd_stdout, "quit EXECUTE for pid = %d with shell_ret(WEXITSTATUS) = %d\n", pid, shell->ret);
	return (shell->ret);
}

int	open_pipe(t_shell *shell)
{
	int		fd[2];
	pid_t	pid;
	int ret;

	pipe(fd);

	pid = fork();
	if (pid == 0)
	{
		// ft_dprintf(shell->fd_stdout, "%sPIPE created [child]\n", GREEN);
		shell->parent = 0;
		// ft_dprintf(shell->fd_stdout, "PIPE_FDs [0] = %d, [1] = %d\n", fd[0], fd[1]);
		ret = dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		// ft_dprintf(shell->fd_stdout, "DUP RET child = %d\n", ret);
		return (0);
	}
	else
	{
		// sleep(1);
		// ft_dprintf(shell->fd_stdout, "ret before pid in PIPE = %d\n", shell->ret);
		waitpid(pid, &shell->ret, 0);
		// ft_dprintf(shell->fd_stdout, "%s", RESET);
		// ft_dprintf(shell->fd_stdout, "ret after pid in PIPE = %d\n", shell->ret);
		ret = dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		// ft_dprintf(shell->fd_stdout, "DUP RET parent = %d\n", ret);


	}
	return (0);
}

int redir_in(t_shell *shell, t_symbol symbol)
{
	int		fdin;
	t_hdoc	*tmp;

	if (symbol == T_REDIRIN)
	{
		// ft_dprintf(shell->fd_stdout, " REDIR IN <[ %s ] for %s\n", pop_key(shell->current), shell->parent? "[parent]" : "[child]");
		fdin = open(pop_key(shell->current), O_RDONLY, 0666);
		if (fdin == -1)
			// ft_dprintf(shell->fd_stdout, "open rd failed: errno = %d\n", errno);
		// ft_dprintf(shell->fd_stdout, "fd = %d\n", fdin);
		if (fdin == -1)
			return (1);
		dup2(fdin, STDIN_FILENO);

		close(fdin);
	}
	else
	{
		// fdin = open(pop_key(shell->current), O_RDONLY, 0666);
		// ft_dprintf(shell->fd_stdout, " REDIR IN << [ %s ] for %s\n", pop_key(shell->current), shell->parent? "[parent]" : "[child]");
		dup2(shell->heredoc->fd, STDIN_FILENO);
		close(shell->heredoc->fd);
		tmp = shell->heredoc;
		shell->heredoc = shell->heredoc->next;
		free(tmp);
	}
	return (0);
}

int redir_out(t_shell *shell, t_symbol symbol)
{
	int	fdout;

	if (symbol == T_REDIROUT)
	{
		// ft_dprintf(shell->fd_stdout, " REDIR OUT > [ %s ] for %s\n", pop_key(shell->current), shell->parent? "[parent]" : "[child]");
		fdout = open(pop_key(shell->current), O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fdout == -1)
			ft_dprintf(shell->fd_stdout, "open wr failed: errno = %d\n", errno);
		// ft_dprintf(shell->fd_stdout, "fd = %d\n", fdout);
	}
	else
	{
		// ft_dprintf(shell->fd_stdout, " APPEND >> [ %s ] for %s\n", pop_key(shell->current), shell->parent? "[parent]" : "[child]");
		fdout = open(pop_key(shell->current), O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fdout == -1)
			ft_dprintf(shell->fd_stdout, "open wr failed: errno = %d\n", errno);
		// ft_dprintf(shell->fd_stdout, "fd = %d\n", fdout);
	}
	if (fdout == -1)
		return (1);
	dup2(fdout, STDOUT_FILENO);
	close(fdout);
	return (0);
}

int	redirect(t_shell *shell, t_symbol symbol)
{
	int ret;

	ret = 0;
	// ft_dprintf(shell->fd_stdout, "errno = %d, shellret = %d before redir for %s\n", errno, shell->ret, shell->parent? "[parent]" : "[child]");
	if (symbol == T_REDIRIN || symbol == T_HEREDOC)
		ret = redir_in(shell, symbol);
	else
		ret = redir_out(shell, symbol);
	// ft_dprintf(shell->fd_stdout, "errno = %d, shellret = %d after redir for %s\n", errno, shell->ret, shell->parent? "[parent]" : "[child]");
	return (ret);
}

int	executor(t_shell *shell)
{
	char		**tab;
	t_symbol	symbol;
	t_symbol	current_exec;
	t_hdoc		*tmp;
	int fd[2];


	tab = NULL;
	// ft_printf("\n%s############ IN EXECUTOR ############%s\n", GREEN, RESET);
	shell->current = shell->start;
	while (shell->current)
	{
		symbol = pop_symbol(shell->current);
		// ft_dprintf(shell->fd_stdout, "\n##### WHILE TOP %s\n", shell->parent? "[parent]" : "[child]");
		// ft_dprintf(shell->fd_stdout, "errno on top= %d, shellret = %d before redir for %s\n", errno, shell->ret, shell->parent? "[parent]" : "[child]");
		if (symbol == T_NEWLINE)
		{
			// ft_dprintf(shell->fd_stdout, "NEWLINE %s\n", shell->parent? "[parent]" : "[child]");
			shell->end = 1;
		}
		if (symbol == T_PIPE)
		{
			if (current_exec == T_BIN)
			{
			// ft_dprintf(shell->fd_stdout, "PIPE(bin) %s\n", shell->parent? "[parent]" : "[child]");
				open_pipe(shell);
				free(tab);
			}
			else if (current_exec == T_BUILTIN)
			{
				// ft_dprintf(shell->fd_stdout, "PIPE(builtin) %s\n", shell->parent? "[parent]" : "[child]");

				pipe(fd);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
				shell->end = 1;
			}
			// ft_dprintf(shell->fd_stdout, "%s quit PIPE\n", shell->parent? "[parent]" : "[child]");
		}
		else if (is_redir(symbol))// && (!shell->parent || shell->end))
		{
			if (!shell->parent || shell->end)
			{
				if (redirect(shell, symbol))
					return (8);
			}
			else if (symbol == T_HEREDOC)
			{
				tmp = shell->heredoc;
				close(tmp->fd);
				shell->heredoc = shell->heredoc->next;
				free(tmp);
			}

		}
		else if (pop_pos(shell->current) == 0 && (!shell->parent || !shell->end))
		{
			tab = get_cmd_tab(shell);
			current_exec = pop_symbol(shell->current);
			// ft_dprintf(shell->fd_stdout, "GET CMD TABt for %s (ptr:%p)\n",shell->parent? "[parent]" : "[child]", tab);
		}
		shell->current = shell->current->next;
		// ft_dprintf(shell->fd_stdout, "##### TOKENlist++ for %s\n", shell->parent? "[parent]" : "[child]");
		if ((!shell->current || pop_pos(shell->current) == 0) && tab && (!shell->parent || shell->end))
		{
			// ft_dprintf(shell->fd_stdout, "%s selected for EXECUTION\n", shell->parent? "[parent]" : "[child]");
			if (current_exec == T_BIN)
				shell->ret = exec_bin(shell, tab);
			else
			{
				shell->ret = exec_builtin(shell, tab);
				// ft_dprintf(shell->fd_stdout, "%s who's executed builtin now listen from [%d]\n", shell->parent? "[parent]" : "[child]", fd[0]);
				dup2(fd[0], STDIN_FILENO);
				dup2(shell->fd_stdout, STDOUT_FILENO);
				free(tab);
				close(fd[0]);
				shell->end = 0;
			}
			// if (!shell->parent)
			// 	exit_success(shell);
			if (shell->ret)
				break;
			tab = NULL;
		}
		// ft_dprintf(shell->fd_stdout, "##### WHILE BOTTOM %s\n", shell->parent? "[parent]" : "[child]");
	}
	// ft_dprintf(shell->fd_stdout, "##### RESET FDs %s\n", shell->parent? "[parent]" : "[child]");

	dup2(shell->fd_stdin, STDIN_FILENO);
	dup2(shell->fd_stdout, STDOUT_FILENO);
	// close (shell->fd_heredoc);
	shell->end = 0;
	return (shell->ret);
}
