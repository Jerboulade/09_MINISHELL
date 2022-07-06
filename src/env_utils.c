/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:16:28 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/05 15:18:53 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env(char *data)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(*new));
	if (!new)
		return (NULL);
	new->str = ft_strdup(data);
	if (!new->str)
		return (NULL);
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

void	add_env(t_shell *shell, t_env *new_env)
{
	t_env	*tmp;

	tmp = shell->senv;
	if (!shell->senv)
		shell->senv = new_env;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_env;
		new_env->prev = tmp;
	}
}

t_env	*get_env_ptr(t_shell *shell, char *name)
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
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

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
