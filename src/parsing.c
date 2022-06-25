/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:36 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/25 00:08:26 by jcarere          ###   ########.fr       */
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

int	get_token(t_shell *shell, int *i)
{
	int ret;

	ft_printf("\nIN GET TOKEN \n");
	if (!is_start(shell->line, *i))
	{
		ret = tokenizer(shell, arg_key(shell, *i), *i);
		if (ret)
			return (ret);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	if (shell->line[*i] && ft_strchr("|><", shell->line[*i]))
	{
		ret = tokenizer(shell, red_key(shell, i), *i);
		if (ret)
			return (ret);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
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
