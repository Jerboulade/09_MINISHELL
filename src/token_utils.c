/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 01:55:11 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:13:28 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_token_pos(t_shell *shell, t_token *token)
{
	if (token->symbol == T_PIPE)
		return (-1);
	else if (is_redir(token->symbol) && pop_pos(shell->current) == -1)
		return (pop_pos(shell->current));
	else if (is_redir(pop_symbol(shell->current)))
		return (pop_pos(shell->current));
	return (pop_pos(shell->current) + 1);
}

void	token_push(t_shell *shell, t_token data)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(*token));
	if (!token)
		exit_free(shell);
	ft_memcpy(token, &data, sizeof(*token));
	shell->current->next = ft_lstnew(token);
	if (!shell->current->next)
		exit_free(shell);
	shell->current = shell->current->next;
}
