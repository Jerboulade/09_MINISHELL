/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:33:15 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:18:51 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	format_move_current_dir_first(char *path)
{
	while (19)
	{
		path = ft_strchr(path, '/');
		if (!path)
			break ;
		while (ft_strncmp(path, "/./", 3) == 0)
			ft_strlcpy(path, path + 2, safe_strlen(path + 2) + 1);
		path++;
	}
}

void	format_shell_path(char *path)
{
	char	*start;
	char	*tmp;

	start = path;
	format_move_current_dir_first(path);
	while (19)
	{
		path = ft_strchr(path, '/');
		if (!path)
			break ;
		while (ft_strncmp(path, "/../", 4) == 0)
		{
			ft_strlcpy(path, path + 3, safe_strlen(path + 3) + 1);
			if (path > start)
				tmp = ft_strreverschr(start, path - 1, '/');
			if (tmp)
				ft_strlcpy(tmp, path, safe_strlen(path) + 1);
			path = tmp;
			tmp = NULL;
		}
		path++;
	}
}

char	**env_listoar(t_shell *shell)
{
	int		i;
	int		size;
	char	**env_array;
	t_env	*tmp;

	tmp = shell->senv;
	if (tmp)
		tmp = tmp->next;
	size = 0;
	while (tmp && ++size)
		tmp = tmp->next;
	env_array = ft_calloc(size + 1, sizeof(*env_array));
	if (!env_array)
		exit_free(shell);
	env_array[size] = NULL;
	tmp = shell->senv;
	if (tmp)
		tmp = tmp->next;
	i = 0;
	while (tmp)
	{
		env_array[i++] = tmp->str;
		tmp = tmp->next;
	}
	return (env_array);
}
