/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:41 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:16:13 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_free(t_shell *shell)
{
	write(STDERR_FILENO, "exit : malloc error\n", 20);
	if (shell)
		free_shell(shell);
	exit(EXIT_FAILURE);
}

void	clear_parsing(t_shell *shell)
{
	if (!shell)
		return ;
	shell->line = NULL;
	if (shell->start && shell->start->next)
		ft_lstclear(&shell->start->next, &free_token);
	free_heredoc(shell);
	dup2(shell->fd_stdin, STDIN_FILENO);
	dup2(shell->fd_stdout, STDOUT_FILENO);
	shell->end = 0;
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->start)
		ft_lstclear(&shell->start, &free_token);
	free_history(shell->history);
	free_env(&shell->senv);
	close(shell->fd_stdin);
	close(shell->fd_stdout);
	free_heredoc(shell);
	free(shell);
}
