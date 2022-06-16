/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:36 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 16:35:57 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*red_key(t_parg *parg, int *i)
{
	char	*key;

	// ft_printf("\nIN RED KEY\n");
	key = NULL;
	parg->pos = -1;
	if (parg->line[*i] == '|')
		key = (ft_strdup("| "));
	else if (parg->line[*i] == '>' && parg->line[*i + 1] != '>')
		key = (ft_strdup("> "));
	else if (parg->line[*i] == '>' && parg->line[*i + 1] == '>')
		key = (ft_strdup(">>"));
	else if (parg->line[*i] == '<' && parg->line[*i + 1] != '<')
		key = (ft_strdup("< "));
	else if (parg->line[*i] == '<' && parg->line[*i + 1] == '<')
		key = (ft_strdup("<<"));
	if (!key)
		return (NULL);
	*i += ft_skipcharlen(key, key[0]);
	return (key);
}

char	*arg_key(t_parg *parg, int start, int end)
{
	char	*key;
	size_t	len;

	// ft_printf("\nIN ARG KEY\n");
	len = end - start;
	key = (char *)ft_calloc(len + 1, sizeof(char));
	if (!key)
		return (NULL);
	ft_strlcpy(key, parg->line + start, len + 1);
	return (key);
}

int	get_token(t_shell *shell, t_parg *parg, int *i, int j)
{
	if (!is_start(parg->line, *i))
	{
		parg->pos++;
		if (tokenizer(shell, parg, arg_key(parg, j, *i)))
			return (parg->ret);
	}
	*i += ft_skipcharlen(parg->line + *i, ' ');
	if (parg->line[*i] && ft_strchr("|><", parg->line[*i]))
	{
		if (tokenizer(shell, parg, red_key(parg, i)))
			return (parg->ret);
	}
	return (0);
}

int	stop_parsing(t_shell *shell, t_parg *parg, int i)
{
	int		pos;
	char	*line;
	t_token	*prev;

	// ft_printf("\nIN STOP PARSING\n");
	// ft_printf("char in stop_parsing = [%c]\n", line[i]);
	pos = parg->pos;
	line = parg->line;
	if (!is_start(line, i))
		prev = (t_token *)shell->current->data;
	if (!line[i] && is_start(line, i))
		return (set_trigger(parg, __FILE__, __LINE__, 0));
	else if (!line[i] && pos == -1 && prev->key[0] != '|')
		return (set_trigger(parg, __FILE__, __LINE__, 1));
	else if (!line[i])
		return (set_trigger(parg, __FILE__, __LINE__, 0));
	else if (is_start(line, i) && ft_strchr("|><", line[i]) && is_end(line, i))
		return (set_trigger(parg, __FILE__, __LINE__, 2));
	else if (is_start(line, i) && line[i] == '|')
		return (set_trigger(parg, __FILE__, __LINE__, 3));
	else if (ft_strchr("|><", line[i]) && pos == -1 && !is_start(line, i))
		return (set_trigger(parg, __FILE__, __LINE__, 4));
	return (0);
}

int	parsing(t_shell *shell, t_parg *parg, int i)
{
	char	quote;
	int		j;

	// ft_printf("\n%s########### IN SPLIT ARG %03d ##########%s\n", GREEN, parg->pos, RESET);
	// ft_printf("starting char = [%c]\n", line[i]);
	quote = 0;
	i += ft_skipcharlen(parg->line + i, ' ');
	if (stop_parsing(shell, parg, i))
		return (print_parserror(parg, i));
	j = i;
	// ft_printf("starting char2 = [%c]\n", parg->line[i]);
	while (parg->line[i])
	{
		// ft_printf("%c, i = %d, j = %d, quote = %c\n", parg->line[j], i, j, quote);
		if (ft_strchr("|> <", parg->line[i]) && !quote)
			break ;
		else if (ft_strchr("\'\"", parg->line[i]))
			set_quote(&quote, parg->line + i);
		i++;
	}
	// ft_printf("char before tokenizer ' ' = [%c]\n", parg->line[i])
	if (get_token(shell, parg, &i, j))
		return (print_parserror(parg, i));
	return (parsing(shell, parg, i));
}
