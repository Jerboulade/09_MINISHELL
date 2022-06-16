/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 17:18:30 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_symbol get_token_symbol(char *key)
{
	if (key[0] == '|')
		return (T_PIPE);
	else if (ft_strchr("<>", key[0]))
		return (T_REDIRECT);
	else if (ft_strequ(key, "echo") || ft_strequ(key, "cd") \
	|| ft_strequ(key, "pwd") || ft_strequ(key, "export") \
	|| ft_strequ(key, "unset") || ft_strequ(key, "env") \
	|| ft_strequ(key, "exit"))
		return (T_BUILTIN);
	// TO DO :	else if (is_valid_command(key))
	//				return (T_COMMAND);
	//			else
	//				return (T_ARG);
	return (T_NO_SYM);
}

int	tokenizer(t_shell *shell, t_parg *parg, char *key)
{
	t_token	*token;

	// ft_printf("\nIN TOKENIZER\n");
	if (!key || !*key)
		return (set_trigger(parg, __FILE__, __LINE__, -1));
	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return (set_trigger(parg, __FILE__, __LINE__, -1));
	token->symbol = get_token_symbol(key);
	token->key = key; // TO DO : token->key = (char *)meta_interpretation(key);
	token->pos = parg->pos;
	// ft_printf("key = %s\n", key);
	if (!*shell->start)
	{
		*shell->start = ft_lstnew(token);
		if (!*shell->start)
			return (set_trigger(parg, __FILE__, __LINE__, -1));
		shell->current = *shell->start;
	}
	else
	{
		shell->current->next = ft_lstnew(token);
		if (!shell->current->next)
			return (set_trigger(parg, __FILE__, __LINE__, -1));
		shell->current = shell->current->next;
	}
	return (0);
}
