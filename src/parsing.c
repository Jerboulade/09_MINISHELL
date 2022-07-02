/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:36 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/02 01:56:08 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*extract_key(t_shell *shell, int end)
{
	int		start;
	char	*key;
	size_t	len;

	// ft_printf("\nIN EXTRACT KEY \n");
	start = get_start_index(shell);
	len = end - start;
	key = ft_calloc(len + 1, sizeof(*key));
	if (!key)
		exit_free(shell);
	ft_strlcpy(key, shell->line + start, len + 1);
	return (key);
}

int	parse_sequence(t_shell *shell, int *i)
{
	int ret;

	// ft_printf("\nIN PARSE SEQUENCE \n");
	// ft_printf("c = %c\n", shell->line[*i]);
	if (!is_start(shell->line, *i) && !is_empty_sequence(shell, *i)) /*!shell->line[*i] ||  !ft_strchr("|><", shell->line[*i])*/
	{
		ret = tokenizer(shell, extract_key(shell, *i), *i);
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
	if (!shell->line[*i] && token_is_redir(pop_token(shell->current)))
		return (4);
	// if (shell->line[*i] && ft_strchr("|><", shell->line[*i]))
	// 	return (2);
	return (0);
}

int	parse_line(t_shell *shell)
{
	int		i;
	int		ret;
	char	quote;

	// ft_printf("\n%s############ IN PARSE LINE ############\n", GREEN, RESET);
	// print_parserror(shell);
	quote = 0;
	i = get_start_index(shell);
	while (1)
	{
		// ft_printf("%s###char =  %c%s\n", GREEN, shell->line[i], RESET);
		if ((!shell->line[i] || ft_strchr("|> <", shell->line[i])) && !quote)
		{
			ret = parse_sequence(shell, &i);
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

t_symbol parser(t_shell *shell)
{
	char *newline;

	// ft_printf("#######################################\n");
	if (!shell->line)
		shell->line = readline(display_prompt(0, 0, NULL, NULL));
	else
	{
		newline = readline(display_prompt(0, 0, NULL, "pipe> "));
		if (!newline)
			return (-1);
		else if (is_empty(newline))
		{
			free(newline);
			return (T_PIPE);
		}
		join_newline(shell, newline);
	}
	// ft_printf("#######################################\n");
	if (!shell->line)				// case : ctrl d
		shell->ret = -1;
	else if (is_empty(shell->line))
		shell->ret = 0;				// case : no command
	else
		shell->ret = parse_line(shell);
	return (pop_symbol(shell->current));
}
