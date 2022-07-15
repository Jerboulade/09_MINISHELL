/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:20:40 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 03:00:40 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else if (ft_strequ("exit", tab[0]))
		ret = msh_exit(shell, tab);
	if (ret)
		return (2);
	return (0);
}

int	exec_bin(t_shell *shell, char **tab, char **env_array)
{
	if (shell->parent)
		g_sig.pid = fork();
	if (g_sig.pid == -1)
		return (print_errno("fork", 1));
	if (g_sig.pid == 0)
	{
		env_array = env_listoar(shell);
		execve(*tab, tab, env_array);
		print_errno(*tab, 126);
		free(env_array);
		free_shell(shell);
		exit(126);
	}
	else
	{
		waitpid(g_sig.pid, &shell->ret, 0);
		if (shell->ret >= 256)
			shell->ret /= 256;
		if (g_sig.signal == 131)
			ft_printf("Quit: %d\n", g_sig.signal - 128);
		if (g_sig.signal)
			shell->ret = g_sig.signal;
	}
	return (shell->ret);
}

void	execute(t_shell *shell, t_symbol exec_type, char **tab, int fdin_pipe)
{
	if (exec_type == T_BIN)
		shell->ret = exec_bin(shell, tab, NULL);
	else
	{
		shell->ret = exec_builtin(shell, tab);
		dup2(fdin_pipe, STDIN_FILENO);
		dup2(shell->fd_stdout, STDOUT_FILENO);
		close(fdin_pipe);
		shell->end = 0;
	}
	free(tab);
	tab = NULL;
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
	update_last_command(shell, tab[0]);
	return (tab);
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
