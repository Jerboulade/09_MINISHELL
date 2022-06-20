/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/20 02:52:09 by jcarere          ###   ########.fr       */
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

t_symbol get_token_symbol(t_shell *shell, int pos, char **key)
{
	if ((*key)[0] == '|')
		return (T_PIPE);
	else if (ft_strchr("<>", (*key)[0]))
		return (T_REDIRECT);
	else if (pos == 0 && (ft_strequ(*key, "echo") || ft_strequ(*key, "cd") \
	|| ft_strequ(*key, "pwd") || ft_strequ(*key, "export") \
	|| ft_strequ(*key, "unset") || ft_strequ(*key, "env") \
	|| ft_strequ(*key, "exit")))
		return (T_BUILTIN);										//	lexer check: shouldn't take key as T_BUILTIN or
	else if (pos == 0 && !(ft_strchr("\'\"./", (*key)[0])))		//				 T_COMMAND if prev is T_REDIRECT
	{
		if (!get_cmd_path(shell, key))
			return (T_INVALID);
		return (T_COMMAND);
	}
	return (T_ARG);
}

int	tokenizer(t_shell *shell, int pos, char *key)
{
	t_token	*token;

	// ft_printf("\nIN TOKENIZER\n pos = %d\n", pos);
	if (!key)
		return (exit_free(shell));
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		return (exit_free(shell));
	token->pos = pos;
	token->symbol = get_token_symbol(shell, pos, &key); // TO DO : built lexer between here and return
	token->key = key; // TO DO : token->key = (char *)meta_interpretation(key);

	// Push token node
	shell->current->next = ft_lstnew(token);
	if (!shell->current->next)
		return (exit_free(shell));
	shell->current = shell->current->next;

	if (token->symbol == T_INVALID)
		return (ret_pars(shell, shell->err_index, 5));
	return (0);
}
