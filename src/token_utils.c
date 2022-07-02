/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 01:55:11 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/02 02:27:52 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_symbol set_token_symbol(char *key)
{
	ft_printf("\nIN GET TOKEN SYMBOL\n");
	if (key[0] == '|')
		return (T_PIPE);
	else if (ft_strchr("<>", key[0]))
	{
		if (ft_strequ(key, "<<"))
			return (T_HEREDOC);
		return (T_REDIRECT);
	}
	return (T_WORD);
}

int set_token_pos(t_shell *shell, t_token *token)
{
	// if (token->symbol == T_PIPE || token->symbol == T_REDIRECT)
	// 	return (-1);
	// if (pop_symbol(shell->current) == T_START)
	// 	return (0);
	if (token->symbol == T_PIPE)
		return (-1);
	else if (token_is_redir(token) && pop_pos(shell->current) == -1)
		return (pop_pos(shell->current));
	else if (token_is_redir(pop_token(shell->current)))
		return (pop_pos(shell->current));
	return (pop_pos(shell->current) + 1);
}

void token_push(t_shell *shell, t_token data)
{
	t_token *token;

	token = ft_calloc(1, sizeof(*token));
	if (!token)
		exit_free(shell);
	ft_memcpy(token, &data, sizeof(*token));
	shell->current->next = ft_lstnew(token);
	if (!shell->current->next)
		exit_free(shell);
	shell->current = shell->current->next;
}

t_token	*pop_token(t_list *node)
{
	return ((t_token *)node->data);
}

char	*pop_key(t_list *node)
{
	return (((t_token *)node->data)->key);
}

t_symbol	pop_symbol(t_list *node)
{
	return (((t_token *)node->data)->symbol);
}

int	pop_index(t_list *node)
{
	return (((t_token *)node->data)->index);
}

int	pop_pos(t_list *node)
{
	return (((t_token *)node->data)->pos);
}
