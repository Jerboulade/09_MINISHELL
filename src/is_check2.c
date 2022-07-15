/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:39:13 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:09:16 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_expandable(char *key)
{
	if (key[0] == '\'' && key[ft_strlen(key) - 1] == '\'')
		return (0);
	return ((ft_strchr(key, '$') != NULL));
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_start(char *line, int i)
{
	if (!i)
		return (1);
	while (--i)
	{
		if (!is_whitespace(line[i]))
			return (0);
	}
	if (!is_whitespace(line[i]))
		return (0);
	return (1);
}

int	is_empty_sequence(t_shell *shell, int i)
{
	int	start;

	start = pop_index(shell->current);
	while (start < i)
	{
		if (!is_whitespace(shell->line[start]))
			return (0);
		++start;
	}
	return (1);
}

int	is_exception(t_shell *shell, char c)
{
	return (pop_symbol(shell->current) == T_PIPE && (c == '<' || c == '>'));
}
