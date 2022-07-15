/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 14:03:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 00:11:33 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace_newkey(t_shell *shell, int *i, char *newkey, char **key)
{
	char	*name;
	char	*var_str;

	name = get_varname(shell, *key + *i + 1);
	var_str = get_env(shell, name);
	ft_memcpy(newkey, var_str, safe_strlen(var_str));
	*key += *i + safe_strlen(name) + 1;
	if (name)
		free(name);
	*i = 0;
	return (safe_strlen(var_str));
}

char	*get_expanded_key(t_shell *shell, char *key, char *newkey)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (key[i])
	{
		if ((key[i] == '\'' || key[i] == '\"'))
		{
			set_quote(&quote, key + i);
			newkey[j++] = key[i++];
		}
		else if (key[i] == '$' && quote != '\'')
			j += replace_newkey(shell, &i, newkey + j, &key);
		else
			newkey[j++] = key[i++];
	}
	return (newkey);
}

int	get_expanded_len(t_shell *shell, char *key)
{
	int		i;
	int		newlen;
	char	quote;

	i = 0;
	quote = 0;
	newlen = 0;
	while (key[i])
	{
		if ((key[i] == '\'' || key[i] == '\"'))
		{
			newlen++;
			set_quote(&quote, key + i++);
		}
		else if (key[i] == '$' && quote != '\'')
			newlen += compute_newlen(shell, &i, &key);
		else
			newlen += (++i != 0);
	}
	return (newlen);
}

void	remove_quote(char *key)
{
	int		i;
	int		move;
	char	quote;

	i = 0;
	move = 0;
	while (key[i])
	{
		if ((key[i] == '\'' || key[i] == '\"'))
		{
			if ((!quote && ft_strchr(key + i + 1, key[i])) || quote == key[i])
				move = 1;
			set_quote(&quote, key + i);
			if (move)
				ft_strlcpy(key + i, key + i + 1, safe_strlen(key + i));
			else
				i++;
			move = 0;
		}
		else
			i++;
	}
}

char	*expand_key(t_shell *shell, char *key)
{
	int		newlen;
	char	*newkey;

	if (ft_strchr(key, '$'))
	{
		newlen = get_expanded_len(shell, key);
		newkey = ft_calloc(newlen + 1, sizeof(*newkey));
		if (!newkey)
			exit_free(shell);
		newkey = get_expanded_key(shell, key, newkey);
		free(key);
		remove_quote(newkey);
	}
	else
	{
		remove_quote(key);
		newkey = key;
	}
	return (newkey);
}
