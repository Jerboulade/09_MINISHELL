/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:51:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/17 03:26:55 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_command(t_shell *shell, t_token *token, char *key)
{
	int			i;
	char		*cmd;
	struct stat	buf;
	// t_symbol	symbol;


	i = -1;
	while (shell->env_path[++i])
	{
		cmd = ft_strjoin(shell->env_path[i], key);
		if (!cmd)
			return (-1); //trigger -1;
		if (stat(cmd, &buf) == 0)
		{
			ft_printf("%s[%02d] '%s' in %s %s: Found\n", GREEN, i + 1, key, shell->env_path[i], RESET);
			token->key = cmd;
			return (0);
		}
		else
		{
			free(cmd);
			ft_printf("%s[%02d] '%s' in %s ", RED, i + 1, key, shell->env_path[i]);
			ft_printf("%s[errno %d]", RESET, errno);
			perror(" ");
		}
	}
	return (1);
}

t_symbol get_token_symbol(t_shell *shell, t_token *token, char *key)
{
	// char *cmd;

	if (key[0] == '|')
		return (T_PIPE);
	else if (ft_strchr("<>", key[0]))
		return (T_REDIRECT);
	else if (ft_strequ(key, "echo") || ft_strequ(key, "cd") \
	|| ft_strequ(key, "pwd") || ft_strequ(key, "export") \
	|| ft_strequ(key, "unset") || ft_strequ(key, "env") \
	|| ft_strequ(key, "exit"))
		return (T_BUILTIN);
	else if (!(ft_strchr("\'\"", key[0]))) // logic condition just to enter statement...here considering that a cmd name won't never start with ' or "
	{
		if (is_valid_command(shell, token, key) == 0) // TO DO change something for malloc error
			return (T_COMMAND);
	}
	// TO DO :	else if (is_valid_command(key))
	//				return (T_COMMAND);
	//			else
	//				return (T_ARG);
	return (T_ARG);
}

int	tokenizer(t_shell *shell, t_parg *parg, char *key)
{
	t_token	*token;

	// ft_printf("\nIN TOKENIZER\n");
	if (!key || !*key)
		return (set_trigger(parg, __FILE__, __LINE__, -1));
	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return (set_trigger(parg, __FILE__, __LINE__, -1));
	token->symbol = get_token_symbol(shell, token, key);
	if ((int)token->symbol == -1)
		set_trigger(parg, __FILE__, __LINE__, -1);
	if (token->symbol != T_COMMAND)
		token->key = key; // TO DO : token->key = (char *)meta_interpretation(key);
	token->pos = parg->pos;
	// ft_printf("key = %s\n", key);
	if (!*shell->start)
	{
		*shell->start = ft_lstnew(token);
		if (!*shell->start)
			return (set_trigger(parg, __FILE__, __LINE__, -1));
		shell->current = *shell->start;
	}
	else
	{
		shell->current->next = ft_lstnew(token);
		if (!shell->current->next)
			return (set_trigger(parg, __FILE__, __LINE__, -1));
		shell->current = shell->current->next;
	}
	return (0);
}
