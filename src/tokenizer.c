/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/25 23:36:34 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_symbol set_token_symbol(char *key)
{
	ft_printf("\nIN GET TOKEN SYMBOL\n");
	if (key[0] == '|')
		return (T_PIPE);
	else if (ft_strchr("<>", key[0]))
	{
		if (ft_strequ(key, "<<"))
			return (T_HEREDOC);
		return (T_REDIRECT);
	}
	return (T_WORD);
}

int set_token_pos(t_shell *shell, t_token *token)
{
	if (token->symbol == T_PIPE || token->symbol == T_REDIRECT)
		return (-1);
	else if (pop_symbol(shell->current) == T_START)
		return (0);
	return (pop_pos(shell->current) + 1);
}

int		in_heredoc(int fd, char *key)
{
	char	*line;

	line = readline(display_prompt(0, 0, NULL, "heredoc> "));
	if (!line || ft_strequ(key, line))
		return (0);
	ft_dprintf(fd, "%s\n", line);
	return (1);

}

void	handle_heredoc(t_shell *shell, t_token *token, char *key)
{
	int fd;

	// ft_printf("\nIN HANDEL HEREDOC\n");
	(void)shell;
	(void)token;
	remove_quote(key);
	fd = open(HEREDOC_PATH, O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		fd = open(HEREDOC_FILE, O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		return ; // at this point, minishell could not open a file des --> check errno
	while (in_heredoc(fd, key))
		continue ;
	close(fd);
}

int	tokenizer(t_shell *shell, char *key, int i)
{
	t_token	*token;

	ft_printf("\nIN TOKENIZER\n");
	if (!key)
		exit_free(shell);
	if (!*key)
		return (1);
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		exit_free(shell);
	token->symbol = set_token_symbol(key);
	if (pop_symbol(shell->current) == T_HEREDOC && token->symbol != T_PIPE && token->symbol != T_REDIRECT)
		handle_heredoc(shell, token, key);
	token->pos = set_token_pos(shell, token);
	token->key = key;
	token->index = i;

	// Push back token node
	shell->current->next = ft_lstnew(token);
	if (!shell->current->next)
		exit_free(shell);

	// Check Parsing error
	// if (token->symbol == T_EMPTY)
	// 	return (3);
	if (token->pos == -1 && pop_pos(shell->current) == -1)
		return (2);
	if (token->symbol == T_PIPE && pop_symbol(shell->current) == T_START)
		return (3);

	shell->current = shell->current->next;
	return (0);
}
