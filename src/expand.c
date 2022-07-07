/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 14:03:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/07 19:26:01 by jcarere          ###   ########.fr       */
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
	// j += safe_strlen(var_str);
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
	// char	*name;
	// char	*var_str;
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
		{
			// name = get_varname(shell, key + i + 1);
			// var_str = get_env(shell, name);
			// ft_memcpy(newkey + j, var_str, safe_strlen(var_str));
			j += replace_newkey(shell, &i, newkey + j, &key);
			// key += i + safe_strlen(name) + 1;
			// if (name)
			// 	free(name);
			// i = 0;
		}
		else
			newkey[j++] = key[i++];
	}
	return (newkey);
}

char	*get_varname(t_shell *shell, const char *key)
{
	size_t	len;
	char	*name;

	if (!key || !*key)
		return (NULL);
	name = (char *)key;
	while (*name)
	{
		if (is_whitespace(*name) || *name == '$' || *name == '\"' || *name == '\'')
			break;
		name++;
	}
	len = name - key;
	name = ft_calloc(len + 1, sizeof(*name));
	name[len] = 0;
	if (!name)
		exit_free(shell);
	ft_memcpy(name, key, len);
	return (name);
}

size_t	safe_strlen(const char *str)
{
	char	*tmp;

	if (!str)
		return (0);
	tmp = (char *)str;
	while (*tmp)
		tmp++;
	return (tmp - str);
}

int	compute_newlen(t_shell *shell, int *i, char **key)
{
	char	*name;
	char	*var_str;
	int		len;

	name = get_varname(shell, *key + *i + 1);
	// ft_printf("name = %s\n", name);
	var_str = get_env(shell, name);
	// ft_printf("var_str = %s\n", var_str);
	len = safe_strlen(var_str);
	// ft_printf("newlen = %d\n", len);
	*key += *i + safe_strlen(name) + 1;
	if (name)
		free(name);
	*i = 0;
	return (len);
}

int	get_expanded_len(t_shell *shell, char *key)
{
	int		i;
	int		newlen;
	// char	*var_str;
	// char	*name;
	char	quote;

	i = 0;
	quote = 0;
	newlen = 0;
	while (key[i])
	{
		// ft_printf("\ncurrent = %s\n", key + i);
		if ((key[i] == '\'' || key[i] == '\"'))
		{
			newlen++;
			set_quote(&quote, key + i++);
			// ft_printf("set quote = [%c]\n", quote);
		}
		else if (key[i] == '$' && quote != '\'')
		{
			newlen += compute_newlen(shell, &i, &key);
			// name = get_varname(shell, key + i + 1);
			// ft_printf("name = %s\n", name);
			// var_str = get_env(shell, name);
			// ft_printf("var_str = %s\n", var_str);
			// newlen += safe_strlen(var_str);
			// ft_printf("newlen = %d\n", newlen);
			// key += i + safe_strlen(name) + 1;
			// if (name)
			// 	free(name);
			// i = 0;
		}
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

	// ft_printf("\nIN EXPAND KEY\n");
	if (ft_strchr(key, '$'))
	{
		newlen = get_expanded_len(shell, key);
		// ft_printf("newlen %d\n", newlen);
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
