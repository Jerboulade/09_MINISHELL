/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:36 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/20 02:51:33 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*red_key(t_shell *shell, int *i)
{
	char	*key;

	// ft_printf("\nIN RED KEY\n");
	key = NULL;
	if (shell->line[*i] == '|')
		key = (ft_strdup("| "));
	else if (shell->line[*i] == '>' && shell->line[*i + 1] != '>')
		key = (ft_strdup("> "));
	else if (shell->line[*i] == '>' && shell->line[*i + 1] == '>')
		key = (ft_strdup(">>"));
	else if (shell->line[*i] == '<' && shell->line[*i + 1] != '<')
		key = (ft_strdup("< "));
	else if (shell->line[*i] == '<' && shell->line[*i + 1] == '<')
		key = (ft_strdup("<<"));
	if (!key)
		return (NULL);
	*i += ft_skipcharlen(key, key[0]);
	return (key);
}

char	*arg_key(t_shell *shell, int start, int end)
{
	char	*key;
	size_t	len;

	// ft_printf("\nIN ARG KEY\n");
	len = end - start;
	key = ft_calloc(len + 1, sizeof(*key));
	if (!key)
		return (NULL);
	ft_strlcpy(key, shell->line + start, len + 1);
	return (key);
}

int	get_token(t_shell *shell, int *pos, int *i, int j)
{
	shell->err_index = *i;
	if (!is_start(shell->line, *i))
	{
		(*pos)++;
		if (tokenizer(shell, *pos, arg_key(shell, j, *i)))
			return (shell->ret);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	shell->err_index = *i;
	if (shell->line[*i] && ft_strchr("|><", shell->line[*i]))
	{
		*pos = -1;
		if (tokenizer(shell, *pos, red_key(shell, i)))
			return (shell->ret);
	}
	return (0);
}

int	stop_parsing(t_shell *shell, int pos, int i)
{
	char	*line;
	t_token	*prev;

	// ft_printf("\nIN STOP PARSING\n");
	// ft_printf("char in stop_parsing = [%c]\n", line[i]);
	line = shell->line;
	if (!is_start(line, i))
		prev = (t_token *)shell->current->data;
	if (!line[i] && is_start(line, i))
		return (ret_pars(shell, i, 0));
	else if (!line[i] && pos == -1 && prev->symbol != T_PIPE)
		return (ret_pars(shell, i, 1));
	else if (!line[i])
		return (ret_pars(shell, i, 0));
	else if (is_start(line, i) && ft_strchr("|><", line[i]) && is_end(line, i))
		return (ret_pars(shell, i, 2));
	else if (is_start(line, i) && line[i] == '|')
		return (ret_pars(shell, i, 3));
	else if (ft_strchr("|><", line[i]) && pos == -1 && !is_start(line, i))
		return (ret_pars(shell, i, 4));
	return (0);
}

int	parsing(t_shell *shell, int pos, int i)
{
	char	quote;
	int		j;

	// ft_printf("\n%s########### IN SPLIT ARG %03d ##########%s\n", GREEN, parg->pos, RESET);
	// ft_printf("starting char = [%c]\n", line[i]);
	quote = 0;
	i += ft_skipcharlen(shell->line + i, ' ');
	if (stop_parsing(shell, pos, i))
		return (shell->ret);
	j = i;
	// ft_printf("starting char2 = [%c]\n", shell->line[i]);
	while (shell->line[i])
	{
		// ft_printf("%c, i = %d, j = %d, quote = %c\n", shell->line[j], i, j, quote);
		if (ft_strchr("|> <", shell->line[i]) && !quote)
			break ;
		else if (ft_strchr("\'\"", shell->line[i]))
			set_quote(&quote, shell->line + i);
		i++;
	}
	// ft_printf("char before tokenizer = [%c]\n", shell->line[i])
	if (get_token(shell, &pos, &i, j))
		return (shell->ret);
	return (parsing(shell, pos, i));
}
