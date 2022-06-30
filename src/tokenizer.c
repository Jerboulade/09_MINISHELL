/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/28 18:53:31 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		in_heredoc(int fd, char *key)
{
	char	*line;

	line = readline(display_prompt(0, 0, NULL, "heredoc> "));
	if (!line || ft_strequ(key, line))
	{
		if (line)
			free(line);
		return (0);
	}
	ft_dprintf(fd, "%s\n", line);
	free(line);
	return (1);

}

int	handle_heredoc(char *key)
{
	int fd;

	// ft_printf("\nIN HANDEL HEREDOC key [%s]\n", key);
	fd = open(HEREDOC_PATH, O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		fd = open(HEREDOC_FILE, O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		return (errno); // at this point, minishell could not open a file des --> check errno
	while (in_heredoc(fd, key))
		continue ;
	close(fd);
	return (0);
}

int	tokenizer(t_shell *shell, char *key, int i)
{
	t_token	token;

	// ft_printf("\nIN TOKENIZER\n");
	if (!key)
		exit_free(shell);
	if (is_expandable(key))
	{
		remove_quote(key);
		key = expand_key(shell, key);
	}
	else
		remove_quote(key);
	token.symbol = T_WORD; //set_token_symbol(key);
	if (pop_symbol(shell->current) == T_HEREDOC && !pop_key(shell->current)) //&& token->symbol != T_PIPE && token->symbol != T_REDIRECT)
		handle_heredoc(key);


	token.key = key; // lexer
	token.index = i;
	token.pos = set_token_pos(shell, &token);
	token_push(shell, token);
	// Check Parsing error
	// if (token->symbol == T_EMPTY)
	// 	return (3);
	// if (token.pos == -1 && pop_pos(shell->current) == -1)
	// 	return (2); //
	// if (token.symbol == T_PIPE && pop_symbol(shell->current) == T_START)
	// 	return (3);
	return (0);
}
