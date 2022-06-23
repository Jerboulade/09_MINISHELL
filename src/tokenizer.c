/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/23 02:03:25 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_path(t_shell *shell, char **key)
{
	int			i;
	char		*path;
	struct stat	buf;

	i = -1;
	while (shell->env_path[++i])
	{
		path = ft_strjoin(shell->env_path[i], *key);
		if (!path)
			return (exit_free(shell));
		if (stat(path, &buf) == 0)
		{
			ft_printf("%s[%02d] '%s' in %s %s: Found\n", GREEN, i + 1, *key, shell->env_path[i], RESET);
			free(*key);
			*key = path;
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

// t_symbol set_token_symbol(t_shell *shell, int pos, char **key)
// {
// 	if ((*key)[0] == '|')
// 		return (T_PIPE);
// 	else if (ft_strchr("<>", (*key)[0]))
// 		return (T_REDIRECT);
// 	else if (pos == 0 && (ft_strequ(*key, "echo") || ft_strequ(*key, "cd") \
// 	|| ft_strequ(*key, "pwd") || ft_strequ(*key, "export") \
// 	|| ft_strequ(*key, "unset") || ft_strequ(*key, "env") \
// 	|| ft_strequ(*key, "exit")))
// 		return (T_BUILTIN);										//	lexer check: shouldn't take key as T_BUILTIN or
// 	else if (pos == 0 && !(ft_strchr("\'\"./", (*key)[0])))		//				 T_COMMAND if prev is T_REDIRECT
// 	{
// 		if (!get_cmd_path(shell, key))
// 			return (T_INVALID);
// 		return (T_COMMAND);
// 	}
// 	return (T_ARG);
// }

t_symbol set_token_symbol(char *key)
{
	// ft_printf("\nIN GET TOKEN SYMBOL\n");
	if (!*key)
		return (T_EMPTY);
	else if (key[0] == '|')
		return (T_PIPE);
	else if (ft_strchr("<>", key[0]))
		return (T_REDIRECT);
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

int	tokenizer(t_shell *shell, char *key, int i)
{
	t_token	*token;

	// ft_printf("\nIN TOKENIZER\n");
	if (!key)
		return (exit_free(shell));
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		return (exit_free(shell));
	token->symbol = set_token_symbol(key);
	token->pos = set_token_pos(shell, token);
	token->key = key;
	token->index = i;

	// Push back token node
	shell->current->next = ft_lstnew(token);
	if (!shell->current->next)
		return (exit_free(shell));

	// Check Parsing error
	if (token->symbol == T_EMPTY)
		return (3);
	if (token->pos == -1 && pop_pos(shell->current) == -1)
		return (2);
	if (token->symbol == T_PIPE && pop_symbol(shell->current) == T_START)
		return (1);

	shell->current = shell->current->next;
	return (0);
}
