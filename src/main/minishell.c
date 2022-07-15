/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 02:59:55 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int signum, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signum == SIGINT)
	{
		if (!g_sig.pid)
			close(STDIN_FILENO);
		else
		{
			waitpid(0, NULL, 0);
			rl_redisplay();
		}
		g_sig.signal = 130;
	}
	else if (signum == SIGQUIT)
	{
		if (!g_sig.pid)
			rl_redisplay();
		else
			waitpid(0, NULL, 0);
		g_sig.signal = 131;
	}
}

int	minishell(t_shell *shell)
{
	shell->ret = -1;
	shell->current = shell->start;
	g_sig.pid = 0;
	g_sig.signal = 0;
	while (parser(shell) == T_PIPE && shell->ret == 0)
		continue ;
	if (g_sig.signal == 130)
		shell->ret = 1;
	if (shell->ret == 0)
		shell->ret = lexer(shell);
	if (shell->ret > 0 && g_sig.signal != 130)
		print_error(shell);
	update_history(shell, shell->history);
	if (shell->ret == 0)
		shell->ret = executor(shell, NULL);
	clear_parsing(shell);
	update_last_exit_status(shell);
	return (shell->ret);
}
