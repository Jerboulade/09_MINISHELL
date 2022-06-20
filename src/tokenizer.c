/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/20 02:03:37 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cmd_path(t_shell *shell, char **key)
{
	int			i;
	char		*path;
	struct stat	buf;
	// t_symbol	symbol;


	i = -1;
	while (shell->env_path[++i])
	{
		path = ft_strjoin(shell->env_path[i], *key);
		if (!path)
			return (exit_free(shell)); //trigger -1;
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
			// free(key);
			// ft_printf("%s[%02d] '%s' in %s ", RED, i + 1, key, shell->env_path[i]);
			// ft_printf("%s[errno %d]", RESET, errno);
			// perror(" ");
		}
	}
	// free(key);
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
		return (T_BUILTIN);
	else if (pos == 0 && !(ft_strchr("\'\"./", (*key)[0]))) // logic condition just to enter statement...here considering that a cmd name won't never start with ' or "
	{
		if (!get_cmd_path(shell, key)) // TO DO change something for malloc error
			return (T_INVALID);
		return (T_COMMAND);
	}
	return (T_ARG);
}

int	tokenizer(t_shell *shell, int pos, char *key)
{
	t_token	*token;

	ft_printf("\nIN TOKENIZER\n pos = %d\n", pos);
	if (!key)
		return (exit_free(shell));
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		return (exit_free(shell));
	token->pos = pos;
	token->symbol = get_token_symbol(shell, pos, &key);
	token->key = key; // TO DO : token->key = (char *)meta_interpretation(key);
	// if (token->symbol == T_INVALID)
	// {
	// 	free(token);
	// 	return (ret_pars(shell, shell->err_index, 5));
	// }
	// if (token->symbol != T_COMMAND)
	// ft_printf("key = %s\n", key);
	// if (!*shell->start)
	// {
	// 	*shell->start = ft_lstnew(token);
	// 	// ft_printf("first node %p\n", shell->start);
	// 	if (!*shell->start)
	// 		return (exit_free(shell));
	// 	shell->current = *shell->start;
	// }
	// else
	// {
	shell->current->next = ft_lstnew(token);
	// ft_printf("next node %p\n", shell->start);
	if (!shell->current->next)
		return (exit_free(shell));
	shell->current = shell->current->next;
	// }
	if (token->symbol == T_INVALID)
		return (ret_pars(shell, shell->err_index, 5));
	return (0);
}
