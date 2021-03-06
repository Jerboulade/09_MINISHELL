/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:29:11 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:13:59 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strreverschr(const char *start, const char *current, char c)
{
	while (current > start)
	{
		if (*current == (char)c)
			return ((char *)current);
		current--;
	}
	if (*current == (char)c)
		return ((char *)current);
	return (NULL);
}

void	join_newline(t_shell *shell, char *newline)
{
	char	*tmp;

	tmp = ft_strjoin(shell->line, newline);
	if (!tmp)
		exit_free(shell);
	free(newline);
	free(shell->line);
	shell->line = tmp;
}

size_t	ft_skipcharlen(const char *str, char c)
{
	char	*tmp;

	if (!str)
		return (0);
	tmp = (char *)str;
	while (*tmp && *tmp == c)
		tmp++;
	return (tmp - str);
}

void	set_quote(char *quote, char *line)
{
	if (!*quote && ft_strchr(line + 1, *line))
		*quote = *line;
	else if (*quote == *line)
		*quote = 0;
}

int	get_start_index(t_shell *shell)
{
	int		start;

	start = pop_index(shell->current);
	start += ft_skipcharlen(shell->line + start, ' ');
	return (start);
}
