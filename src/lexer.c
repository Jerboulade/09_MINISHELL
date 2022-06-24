/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 21:01:48 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/24 03:30:39 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			ft_printf("%s[%02d] '%s' in %s %s: Found\n", GREEN, i + 1, token->key, shell->env_path[i], RESET);
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

void	decrement_tokenpos(t_list *next)
{
	while (next && pop_token(next)->pos != -1)
	{
		pop_token(next)->pos--;
		next = next->next;
	}
}

int		is_builtin(char *key)
{
	return ((ft_strequ(key, "echo") || ft_strequ(key, "cd") \
		|| ft_strequ(key, "pwd") || ft_strequ(key, "export") \
		|| ft_strequ(key, "unset") || ft_strequ(key, "env") \
		|| ft_strequ(key, "exit")));
}

void	remove_quote(char *key)
{
	int	i;

	if (!(ft_strchr("\'\"", key[0])))
		return ;
	i = 0;
	*(ft_strrchr(key, key[0])) = 0;
	while (key[++i])
		key[i - 1] = key[i];
	key[i - 1] = 0;
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

int	lexer(t_shell *shell)
{
	ft_printf("%s#######################################\n", CYAN);
	ft_printf("                  LEXER                  \n");
	ft_printf("#######################################%s\n", RESET);

	if (pop_symbol(shell->current) == T_REDIRECT)
		return (5);
	shell->current = shell->start;
	while (shell->current)
	{
		if (pop_symbol(shell->current) == T_WORD)
		{
			remove_quote(pop_key(shell->current));
			if (pop_pos(shell->current) == 0)// if pos = 0
			{
				if (is_builtin(pop_key(shell->current))) //if builtin -> T_BUILTIN
					pop_token(shell->current)->symbol = T_BUILTIN;
				else if (is_existing_bin(shell, pop_token(shell->current)))// else check if valid cmd -> T_COMMAND + path
					pop_token(shell->current)->symbol = T_BIN;
				else  // else return 7
					return (6);
			}
		}
		else if (pop_symbol(shell->current) == T_REDIRECT)
		{
			pop_token(shell->current->next)->symbol = T_FILE;
		}
		else if (pop_symbol(shell->current) == T_FILE)
		{
			remove_quote(pop_key(shell->current));
			if (shell->current->next)
				decrement_tokenpos(shell->current->next);
			if (is_dir(pop_key(shell->current)))
				return (7);
			// check if not dir : return 6
			//       else intepret quote
		}

		shell->current = shell->current->next;
	}
	return (0);
}
