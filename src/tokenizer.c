/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/14 20:01:32 by jcarere          ###   ########.fr       */
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

int		in_heredoc(t_shell *shell, char *key, int fd)
{
	char	*line;

	line = readline("heredoc> ");
	if (!line || ft_strequ(key, line))
	{
		if (line)
			free(line);
		return (0);
	}
	line = expand_key(shell, line);
	ft_dprintf(fd, "%s\n", line);
	free(line);
	return (1);

}

int	handle_heredoc(t_shell *shell, char *key)
{
	int		fd[2];
	t_hdoc	*heredoc;
	t_hdoc	*tmp;

	heredoc = ft_calloc(1, sizeof(*heredoc));
	if (!heredoc)
		exit_free(shell);
	pipe(fd);
	// close(shell->fd_heredoc);
	while (in_heredoc(shell, key, fd[1]))
		continue ;
	close(fd[1]);
	heredoc->fd = fd[0];
	if (!shell->heredoc)
		shell->heredoc = heredoc;
	else
	{
		tmp = shell->heredoc;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = heredoc;
	}
	if (sig.signal == 130)
		return (1);
	return (0);
}

size_t	 meta_token(t_shell *shell, int i)
{
	size_t	size;
	t_token	token;

	// ft_printf("\nIN META TOKEN\n");
	size = 1;
	if (shell->line[i] == '|')
	{
		token.symbol = T_PIPE;
		shell->piped = 1;
	}
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

	if (!key)
		exit_free(shell);
	key = expand_key(shell, key);
	token.symbol = T_WORD;
	if (pop_symbol(shell->current) == T_HEREDOC && !pop_key(shell->current))
	{
		if (handle_heredoc(shell, key))
			return (1);
	}
	token.key = key;
	token.index = i;
	token.pos = set_token_pos(shell, &token);
	token_push(shell, token);
	return (0);
}
