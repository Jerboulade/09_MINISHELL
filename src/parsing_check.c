/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:43:01 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/02 02:17:52 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_is_redir(t_token *token)
{
	return (token->symbol == T_REDIRIN \
			|| token->symbol == T_REDIROUT \
			|| token->symbol == T_APPEND \
			|| token->symbol == T_HEREDOC);
}

int	last_token_is_meta(t_shell *shell)
{
	return (pop_symbol(shell->current) == T_REDIRIN \
			|| pop_symbol(shell->current) == T_REDIROUT \
			|| pop_symbol(shell->current) == T_REDIRIN \
			|| pop_symbol(shell->current) == T_HEREDOC \
			|| pop_symbol(shell->current) == T_PIPE);
}

int	is_empty_sequence(t_shell *shell, int i)
{
	int start;

	start = pop_index(shell->current);
	while (start < i)
	{
		if (shell->line[start] != ' ') // is_whitespace
			return (0);
		++start;
	}
	return (1);
}

int	is_exception(t_shell *shell, char c)
{
	return (pop_symbol(shell->current) == T_PIPE && c == '<');
}
