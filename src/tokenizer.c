/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/23 21:02:36 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_symbol set_token_symbol(char *key)
{
	// ft_printf("\nIN GET TOKEN SYMBOL\n");
	if (!*key)
		return (T_EMPTY);
	else if (key[0] == '|')
		return (T_PIPE);
	else if (ft_strchr("<>", key[0]))
		return (T_REDIRECT);
	return (T_WORD);
}

int set_token_pos(t_shell *shell, t_token *token)
{
	if (token->symbol == T_PIPE || token->symbol == T_REDIRECT)
		return (-1);
	else if (pop_symbol(shell->current) == T_START)
		return (0);
	return (pop_pos(shell->current) + 1);
}

int	tokenizer(t_shell *shell, char *key, int i)
{
	t_token	*token;

	// ft_printf("\nIN TOKENIZER\n");
	if (!key)
		return (exit_free(shell));
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		return (exit_free(shell));
	token->symbol = set_token_symbol(key);
	token->pos = set_token_pos(shell, token);
	token->key = key;
	token->index = i;

	// Push back token node
	shell->current->next = ft_lstnew(token);
	if (!shell->current->next)
		return (exit_free(shell));

	// Check Parsing error
	if (token->symbol == T_EMPTY)
		return (3);
	if (token->pos == -1 && pop_pos(shell->current) == -1)
		return (2);
	if (token->symbol == T_PIPE && pop_symbol(shell->current) == T_START)
		return (1);

	shell->current = shell->current->next;
	return (0);
}
