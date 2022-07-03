/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/03 02:55:15 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_add_newline(t_shell *shell)
{
	t_token token;

	token.pos = -1;
	token.key = NULL;
	token.index = pop_index(shell->current) + 1;
	token.symbol = T_NEWLINE;
	token_push(shell, token);
}

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
		return (errno); // at this point, minishell could not open a fd --> check errno
	while (in_heredoc(fd, key))
		continue ;
	close(fd);
	return (0);
}

size_t	 meta_token(t_shell *shell, int i)
{
	size_t	size;
	t_token	token;

	// ft_printf("\nIN META TOKEN\n");
	size = 1;
	if (shell->line[i] == '|')
		token.symbol = T_PIPE;
	else if (shell->line[i] == '>' && shell->line[i + 1] != '>')
		token.symbol = T_REDIROUT;
	else if (shell->line[i] == '>' && shell->line[i + 1] == '>' && ++size)
		token.symbol = T_APPEND;
	else if (shell->line[i] == '<' && shell->line[i + 1] != '<')
		token.symbol = T_REDIRIN;
	else if (shell->line[i] == '<' && shell->line[i + 1] == '<' && ++size)
		token.symbol = T_HEREDOC;
	token.index = i + size;
	token.key = NULL;
	token.pos = set_token_pos(shell, &token);
	token_push(shell, token);
	return (size);
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
	token.symbol = T_WORD;
	if (pop_symbol(shell->current) == T_HEREDOC && !pop_key(shell->current))
		handle_heredoc(key);
	token.key = key;
	token.index = i;
	token.pos = set_token_pos(shell, &token);
	token_push(shell, token);
	return (0);
}
