/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 14:03:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/04 16:53:02 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(t_shell *shell, char *name)
{
	size_t	len;
	t_env	*tmp;

	if (!name)
		return (NULL);
	len = ft_strlen(name);
	tmp = shell->senv;
	while (tmp)
	{
		if (ft_strncmp(name, tmp->str, len) == 0 && tmp->str[len] == '=')
			return (tmp->str + len + 1);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_expanded_key(t_shell *shell, char *key, char *newkey)
{
	int		i;
	int		j;
	char	*name;
	char	*var_str;

	i = 0;
	j = 0;
	while (key[i])
	{
		if (key[i] == '$')
		{
			name = get_varname(shell, key + i + 1);
			var_str = get_env(shell, name);
			ft_memcpy(newkey + j, var_str, safe_strlen(var_str));
			j += safe_strlen(var_str);
			key += i + safe_strlen(name) + 1;
			if (name)
				free(name);
			i = 0;
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
		if (is_whitespace(*name) || *name == '$')
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

int	get_expanded_len(t_shell *shell, char *key)
{
	int		i;
	int		newlen;
	char	*var_str;
	char	*name;

	i = 0;
	newlen = 0;
	while (key[i])
	{
		if (key[i] == '$')
		{
			name = get_varname(shell, key + i + 1);
			var_str = get_env(shell, name);
			newlen += safe_strlen(var_str);
			key += i + safe_strlen(name) + 1;
			if (name)
				free(name);
			i = 0;
		}
		else
			newlen += (++i != 0);
	}
	return (newlen);
}

char	*expand_key(t_shell *shell, char *key)
{
	int		newlen;
	char	*newkey;

	// ft_printf("\nIN EXPAND KEY\n");
	newlen = get_expanded_len(shell, key);
	newkey = ft_calloc(newlen + 1, sizeof(*newkey));
	if (!newkey)
		exit_free(shell);
	newkey = get_expanded_key(shell, key, newkey);
	free(key);
	return (newkey);
}
