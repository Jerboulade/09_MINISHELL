/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_pop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 01:21:17 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/03 01:24:31 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
