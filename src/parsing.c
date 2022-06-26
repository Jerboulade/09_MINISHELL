/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:36 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/26 22:39:23 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*red_key(t_shell *shell, int *i)
{
	char	*key;

	ft_printf("\nIN RED KEY\n");
	key = NULL;
	if (shell->line[*i] == '|')
		key = (ft_strdup("| "));
	else if (shell->line[*i] == '>' && shell->line[*i + 1] != '>')
		key = (ft_strdup(">"));
	else if (shell->line[*i] == '>' && shell->line[*i + 1] == '>')
		key = (ft_strdup(">>"));
	else if (shell->line[*i] == '<' && shell->line[*i + 1] != '<')
		key = (ft_strdup("<"));
	else if (shell->line[*i] == '<' && shell->line[*i + 1] == '<')
		key = (ft_strdup("<<"));
	if (!key)
		return (NULL);
	*i += ft_skipcharlen(key, key[0]);
	return (key);
}

char	*arg_key(t_shell *shell, int end)
{
	int		start;
	char	*key;
	size_t	len;

	ft_printf("\nIN ARG KEY \n");
	start = get_start_index(shell);
	len = end - start;
	key = ft_calloc(len + 1, sizeof(*key));
	if (!key)
		exit_free(shell);
	ft_strlcpy(key, shell->line + start, len + 1);
	return (key);
}

void token_push(t_shell *shell, t_token data)
{
	t_token *token;

	token = ft_calloc(1, sizeof(*token));
	if (!token)
		exit_free(shell);
	ft_memcpy(token, &data, sizeof(*token));
	shell->current->next = ft_lstnew(token);
	if (!shell->current->next)
		exit_free(shell);
	shell->current = shell->current->next;
}

size_t	 meta_token(t_shell *shell, int i)
{
	size_t	size;
	t_token	token;

	ft_printf("\nIN META TOKEN\n");
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
	token.pos = -1;
	token.index = i + size;
	token.key = NULL;
	token_push(shell, token);
	return (size);
}

int	token_is_redir(t_shell *shell)
{
	return (pop_symbol(shell->current) == T_REDIRIN \
			|| pop_symbol(shell->current) == T_REDIROUT \
			|| pop_symbol(shell->current) == T_REDIRIN \
			|| pop_symbol(shell->current) == T_HEREDOC);
}

int	last_token_is_meta(t_shell *shell)
{
	return (pop_symbol(shell->current) == T_REDIRIN \
			|| pop_symbol(shell->current) == T_REDIROUT \
			|| pop_symbol(shell->current) == T_REDIRIN \
			|| pop_symbol(shell->current) == T_HEREDOC \
			|| pop_symbol(shell->current) == T_PIPE);
}

int	arg_is_empty(t_shell *shell, int i)
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

int	get_token(t_shell *shell, int *i)
{
	int ret;

	ft_printf("\nIN GET TOKEN \n");
	ft_printf("c = %c\n", shell->line[*i]);
	if (!is_start(shell->line, *i) && !arg_is_empty(shell, *i)) /*!shell->line[*i] ||  !ft_strchr("|><", shell->line[*i])*/
	{
		ret = tokenizer(shell, arg_key(shell, *i), *i);
		if (ret)
			return (ret);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	if (shell->line[*i] && ft_strchr("|><", shell->line[*i]))
	{
		if (shell->line[*i] == '|' && pop_symbol(shell->current) == T_START)
			return (2);
		if (last_token_is_meta(shell) && !is_exception(shell, shell->line[*i]))
			return (3);
		*i += meta_token(shell, *i);
		// ret = tokenizer(shell, red_key(shell, i), *i);
		// if (ret)
		// 	return (ret);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	if (!shell->line[*i] && token_is_redir(shell))
		return (4);
	// if (shell->line[*i] && ft_strchr("|><", shell->line[*i]))
	// 	return (2);
	return (0);
}

int	parsing(t_shell *shell)
{
	int		i;
	int		ret;
	char	quote;

	ft_printf("\n%s############## IN PARSING ############\n", GREEN, RESET);
	print_parserror(shell);
	quote = 0;
	i = get_start_index(shell);
	while (1)
	{
		// ft_printf("%s###char =  %c%s\n", GREEN, shell->line[i], RESET);
		if ((!shell->line[i] || ft_strchr("|> <", shell->line[i])) && !quote)
		{
			ret = get_token(shell, &i);
			if (ret != 0 || !shell->line[i])
				return (ret);
		}
		else if (ft_strchr("\'\"", shell->line[i]))
		{
			set_quote(&quote, shell->line + i);
			i++;
		}
		else
			i++;
	}
	return (0);
}
