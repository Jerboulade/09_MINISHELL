/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 15:52:37 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenizer(t_shell *shell, t_parg *parg, char *key)
{
	t_token	*token;

	// ft_printf("\nIN TOKENIZER\n");
	if (!key || !*key)
		return (error_trigger(parg, __FILE__, __LINE__, -1));
	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return (error_trigger(parg, __FILE__, __LINE__, -1));
	token->key = key; // TO DO : token->key = meta_interpretation(token->key);
	token->pos = parg->pos;
	token->symbol = T_TEST; // TO DO : token->symbol = get_token_symbol(token);
	if (!parg->pos)
		token->symbol = T_COMMAND;
	// ft_printf("key = %s\n", key);
	if (!*shell->start)
	{
		*shell->start = ft_lstnew(token);
		if (!*shell->start)
			return (error_trigger(parg, __FILE__, __LINE__, -1));
		shell->current = *shell->start;
	}
	else
	{
		shell->current->next = ft_lstnew(token);
		if (!shell->current->next)
			return (error_trigger(parg, __FILE__, __LINE__, -1));
		shell->current = shell->current->next;
	}
	return (0);
}
