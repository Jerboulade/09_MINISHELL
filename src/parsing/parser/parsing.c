/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:36 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:10:33 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_key(t_shell *shell, int end)
{
	int		start;
	char	*key;
	size_t	len;

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
	int	ret;

	if (!is_start(shell->line, *i) && !is_empty_sequence(shell, *i))
	{
		ret = tokenizer(shell, extract_key(shell, *i), *i);
		if (ret)
			return (ret);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	*i += ft_skipcharlen(shell->line + *i, '	');
	if (shell->line[*i] && ft_strchr("|><", shell->line[*i]))
	{
		if (shell->line[*i] == '|' && pop_symbol(shell->current) == T_START)
			return (2);
		if (is_meta(pop_symbol(shell->current)) \
			&& !is_exception(shell, shell->line[*i]))
			return (3);
		*i += meta_token(shell, *i);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	*i += ft_skipcharlen(shell->line + *i, '	');
	if (!shell->line[*i] && is_redir(pop_symbol(shell->current)))
		return (4);
	return (0);
}

int	parse_line(t_shell *shell)
{
	int		i;
	int		ret;
	char	quote;

	quote = 0;
	i = get_start_index(shell);
	while (1)
	{
		if ((!shell->line[i] || ft_strchr("|> 	<", shell->line[i])) && !quote)
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

char	*display_prompt(int i, int j, char *tmp, t_shell *shell)
{
	static char	prompt[PROMPT_SIZE + 42];

	tmp = get_env(shell, "USER");
	j += ft_strlcpy(prompt + j, MAG, 10);
	while (tmp && tmp[i] && j < PROMPT_SIZE + 9)
		prompt[j++] = tmp[i++];
	i = 0;
	tmp = get_env(shell, "HOSTNAME");
	j += ft_strlcpy(prompt + j, GREEN, 10);
	if (tmp)
		prompt[j++] = '@';
	while (tmp && tmp[i] && j < PROMPT_SIZE + 19)
		prompt[j++] = tmp[i++];
	i = 0;
	tmp = get_env(shell, "PWD");
	if (tmp)
		tmp = ft_strrchr(tmp, '/') + (ft_strrchr(tmp, '/') != NULL);
	j += ft_strlcpy(prompt + j, CYAN, 10);
	if (tmp)
		prompt[j++] = ':';
	while (tmp && tmp[i] && j < PROMPT_SIZE + 29)
		prompt[j++] = tmp[i++];
	j += ft_strlcpy(prompt + j, RESET, 10);
	ft_strlcpy(prompt + j, "> ", 3);
	return (prompt);
}

t_symbol	parser(t_shell *shell)
{
	char	*newline;

	if (!shell->line)
		shell->line = readline(display_prompt(0, 0, NULL, shell));
	else
	{
		newline = readline("pipe> ");
		if (!newline)
			return (-1);
		else if (is_empty(newline))
		{
			free(newline);
			return (T_PIPE);
		}
		join_newline(shell, newline);
	}
	if (!shell->line)
		shell->ret = -1;
	else if (is_empty(shell->line))
		shell->ret = 0;
	else
		shell->ret = parse_line(shell);
	return (pop_symbol(shell->current));
}
