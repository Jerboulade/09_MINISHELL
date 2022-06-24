/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:29:11 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/23 04:49:51 by jcarere          ###   ########.fr       */
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

int is_empty(const char *line)
{
	// ft_printf("\nIN IS EMPTY\n");
	if (!line || !*line)
		return (1);
	while (*line)
	{
		if (*line != ' ')
			return (0);
		++line;
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

// int	ret_pars(t_shell *shell, int i, int ret)
// {
// 	// ft_printf("\nIN ret_pars\n");
// 	// ft_printf("ret = %d\n", ret);
// 	shell->err_index = i;
// 	shell->ret = ret;
// 	// ft_printf("IF (RET)\n");
// 	// ft_printf("PARG: ret[%d] pos[%d]\n", parg->ret, parg->pos);
// 	return (1);
// }

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

int	 get_start_index(t_shell *shell)
{
	int		start;

	start = pop_index(shell->current);
	start += ft_skipcharlen(shell->line + start, ' ');
	return (start);
}
