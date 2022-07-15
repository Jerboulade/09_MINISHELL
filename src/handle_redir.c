/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:11:02 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 00:22:19 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_in(t_shell *shell, t_symbol symbol)
{
	int		fdin;
	char	*path;
	t_hdoc	*tmp;

	path = pop_key(shell->current);
	if (symbol == T_REDIRIN)
	{
		fdin = open(path, O_RDONLY, 0666);
		if (fdin == -1 || dup2(fdin, STDIN_FILENO) == -1)
			shell->ret = print_errno(path, 1);
		if (fdin != -1)
			close(fdin);
	}
	else
	{
		dup2(shell->heredoc->fd, STDIN_FILENO);
		close(shell->heredoc->fd);
		tmp = shell->heredoc;
		shell->heredoc = shell->heredoc->next;
		free(tmp);
	}
}

void	redir_out(t_shell *shell, t_symbol symbol)
{
	int		fdout;
	char	*path;

	path = pop_key(shell->current);
	if (symbol == T_REDIROUT)
	{
		fdout = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fdout == -1)
			shell->ret = print_errno(path, 1);
	}
	else
	{
		fdout = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fdout == -1)
			shell->ret = print_errno(path, 1);
	}
	if (!shell->ret && dup2(fdout, STDOUT_FILENO) == -1)
		shell->ret = print_errno(path, 1);
	if (fdout != -1)
		close(fdout);
}

void	handle_redir(t_shell *shell, t_symbol symbol, char **tab)
{
	t_hdoc	*tmp;

	if (!shell->parent || shell->end)
	{
		if (symbol == T_REDIRIN || symbol == T_HEREDOC)
			redir_in(shell, symbol);
		else
			redir_out(shell, symbol);
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
