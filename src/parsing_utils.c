/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:29:11 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 16:14:38 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_skipcharlen(const char *str, char c)
{
	char	*tmp;

	if (!str)
		return (0);
	tmp = (char *)str;
	while (*str && *tmp == c)
		tmp++;
	return (tmp - str);
}

int	is_end(char *line, int i)
{
	if (!line[i])
		return (1);
	while (line[++i])
	{
		if (line[i] != ' ')
			return (0);
	}
	return (1);
}

int	is_start(char *line, int i)
{
	if (!i)
		return (1);
	while (--i)
	{
		if (line[i] != ' ')
			return (0);
	}
	if (line[i] != ' ')
		return (0);
	return (1);
}

int	error_trigger(t_parg *parg, char *file, int errline, int ret)
{
	// ft_printf("\nIN ERROR TRIGGER\n");
	// ft_printf("PARG: ret[%d] pos[%d]\n", parg->ret, parg->pos);
	if (ret)
	{
		parg->ret = ret;
		parg->error_line = errline;
		parg->error_file = file;
	}
	// ft_printf("IF (RET)\n");
	// ft_printf("PARG: ret[%d] pos[%d]\n", parg->ret, parg->pos);
	return (1);
}

void	set_quote(char *quote, char *line)
{
	// ft_printf("\nIN SET QUOTE\n");
	// ft_printf("quote = [%c]\nc = [%c]\n", *quote, *line);
	if (!*quote && ft_strchr(line + 1, *line))
		*quote = *line;
	else if (*quote == *line)
		*quote = 0;
	// ft_printf("new quote = [%c]\nc = [%c]\n", *quote, *line);
}
