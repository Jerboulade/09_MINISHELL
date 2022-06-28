/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 14:03:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/28 14:27:26 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_expanded_var(char **env, char *name, int len)
{
	int i;

	i = -1;
	if (!name || !len)
		return (NULL);
	// ft_printf("name = %s, len = %d\n", name, len);
	while (env[++i])
	{
		if (ft_strncmp(name, env[i], len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
	}
	return (NULL);
}

size_t	var_len(const char *var_start)
{
	char *tmp;

	if (!var_start || !*var_start)
		return (0);
	tmp = (char *)var_start;
	while (*tmp)
	{
		if (is_whitespace(*tmp) || *tmp == '$')
			return (tmp - var_start);
		tmp++;
	}
	return (tmp - var_start);
}

char	*get_expanded_key(t_shell *shell, char *key, char *newkey)
{
	int i;
	int j;
	int len;
	char *var_string;

	i = 0;
	j = 0;
	while (key[i])
	{
		// ft_printf("loop2: key[i] = %c\n", key[i]);
		// ft_printf("loop2: newkey[%d] = %c\n",j, newkey[j ? j - 1 : j]);
		if (key[i] == '$')
		{
			len = var_len(key + i + 1);
			var_string = get_expanded_var(shell->env, key + i + 1, len);
			if (var_string)
			{
				ft_memcpy(newkey + j, var_string, ft_strlen(var_string));
				j += ft_strlen(var_string);
			}
			key += i + len + 1;
			i = 0;
		}
		else
			newkey[j++] = key[i++];
	}
	printf("newkey %s, j = %d\n", newkey, j);
	return (newkey);
}

int	get_expanded_len(t_shell *shell, char *key)
{
	int i;
	int len;
	int newlen;
	char *var_string;

	i = 0;
	newlen = 0;
	while (key[i])
	{
		// ft_printf("loop: key[i] = %c\n", key[i]);
		if (key[i] == '$')
		{
			len = var_len(key + i + 1);
			var_string = get_expanded_var(shell->env, key + i + 1, len);
			if (var_string)
				newlen += ft_strlen(var_string);
			// newlen += i;
			// ft_printf("VAR = %s\n", var_string);
			key += i + len + 1;
			i = 0;
		}
		else
			newlen += (++i != 0);
	}
	ft_printf("newlen = %d\n", newlen);
	return (newlen);
}

char	*expand_key(t_shell *shell, char *key)
{
	int		newlen;
	char	*newkey;

	ft_printf("\nIN EXPAND KEY\n");
	newlen = get_expanded_len(shell, key);
	newkey = ft_calloc(newlen + 1, sizeof(*newkey));
	if (!newkey)
		exit_free(shell);
	newkey = get_expanded_key(shell, key, newkey);
	free(key);
	return (newkey);
}
