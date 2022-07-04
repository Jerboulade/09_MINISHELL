/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 01:12:40 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/04 22:29:01 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_flag(t_symbol symbol)
{
	return (symbol == T_PIPE || symbol == T_NEWLINE);
}

int	is_redir(t_symbol symbol)
{
	return (symbol == T_REDIRIN \
			|| symbol == T_REDIROUT \
			|| symbol == T_APPEND \
			|| symbol == T_HEREDOC);
}

int	is_meta(t_symbol symbol)
{
	return (symbol == T_REDIRIN \
			|| symbol == T_REDIROUT \
			|| symbol == T_REDIRIN \
			|| symbol == T_HEREDOC \
			|| symbol == T_PIPE);
}

int	is_relbin(char *key)
{
	struct stat	info;

	if (stat(key, &info) == 0 && (info.st_mode & S_IFMT) != S_IFDIR)
	{
		ft_printf("%s is (S_KEY = %d)\n", key, info.st_mode & S_IFMT);
		ft_printf("S_IFDIR = %d\nS_IFREG = %d\nS_IFCHR = %d\nS_IFBLK = %d\nS_IFLNK = %d\nS_IFIFO = %d\nS_IFSOCK = %d\n", S_IFDIR, S_IFREG, S_IFCHR, S_IFBLK, S_IFLNK, S_IFIFO, S_IFSOCK);
		return (1);
	}
	return (0);
}

int		is_builtin(char *key)
{
	return ((ft_strequ(key, "echo") || ft_strequ(key, "cd") \
		|| ft_strequ(key, "pwd") || ft_strequ(key, "export") \
		|| ft_strequ(key, "unset") || ft_strequ(key, "env") \
		|| ft_strequ(key, "exit")));
}

int is_expandable(char *key)
{
	if (key[0] == '\'' && key[ft_strlen(key) - 1] == '\'')
		return (0);
	return ((ft_strchr(key, '$') != NULL));
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

int is_empty(const char *line)
{
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

int	is_empty_sequence(t_shell *shell, int i)
{
	int start;

	start = pop_index(shell->current);
	while (start < i)
	{
		if (shell->line[start] != ' ') // is_whitespace
			return (0);
		++start;
	}
	return (1);
}

int	is_exception(t_shell *shell, char c)
{
	return (pop_symbol(shell->current) == T_PIPE && (c == '<' || c == '>'));
}
