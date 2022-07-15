/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 22:55:34 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:17:28 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_varname(t_shell *shell, const char *key)
{
	size_t	len;
	char	*name;

	if (!key || !*key)
		return (NULL);
	name = (char *)key;
	while (*name)
	{
		if (is_whitespace(*name) || *name == '$' \
			|| *name == '\"' || *name == '\'')
			break ;
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
	var_str = get_env(shell, name);
	len = safe_strlen(var_str);
	*key += *i + safe_strlen(name) + 1;
	if (name)
		free(name);
	*i = 0;
	return (len);
}
