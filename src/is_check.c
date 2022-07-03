/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 01:12:40 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/03 02:37:57 by jcarere          ###   ########.fr       */
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

int	is_existing_bin(t_shell *shell, t_token *token)
{
	int			i;
	char		*path;
	struct stat	buf;

	i = -1;
	while (shell->env_path[++i])
	{
		path = ft_strjoin(shell->env_path[i], token->key);
		if (!path)
			return (exit_free(shell));
		if (stat(path, &buf) == 0)
		{
			// ft_printf("%s[%02d] '%s' in %s %s: Found\n", GREEN, i + 1, token->key, shell->env_path[i], RESET);
			free(token->key);
			token->key = path;
			return (1);
		}
		else
		{
			free(path);
			// ft_printf("%s[%02d] '%s' in %s ", RED, i + 1, key, shell->env_path[i]);
			// ft_printf("%s[errno %d]", RESET, errno);
			// perror(" ");
		}
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

int	is_dir(const char *key)
{
	int fd;

	fd = open(key, O_RDWR | O_CREAT, 0666);
	// if (fd == -1)
	// {
	// struct stat statbuf;
	//
	// stat(key, &statbuf);
	ft_printf("errno : %d", errno);
	perror("error: ");
	close(fd);
	if (fd == -1)
		return (1);
	return (0);
	// ft_printf("statbuf.st_mode = %d\nS_IFDIR = %d\nS_IFMT = %d\nS_IFREG = %d\n", statbuf.st_mode, S_IFDIR, S_IFMT, S_IFREG);
	// return (statbuf.st_mode == S_IFDIR);
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
	return (pop_symbol(shell->current) == T_PIPE && c == '<');
}
