/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:01:58 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:13:04 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**build_pathtab(t_shell *shell, char *key)
{
	int		i;
	char	*tmp;
	char	**path_tab;
	char	*paths;

	paths = get_env(shell, "PATH");
	if (!paths)
		return (NULL);
	path_tab = ft_split(paths, ':');
	if (!path_tab)
		exit_free(shell);
	if (!append_backslash(path_tab))
		exit_free(shell);
	i = -1;
	while (path_tab[++i])
	{
		tmp = ft_strjoin(path_tab[i], key);
		if (!tmp)
			exit_free(shell);
		free(path_tab[i]);
		path_tab[i] = tmp;
	}
	return (path_tab);
}

int	check_bin_path(char *path)
{
	int			stat_ret;
	struct stat	info;

	stat_ret = stat(path, &info);
	if (stat_ret != 0)
		return (127);
	if (stat_ret == 0 && (info.st_mode & S_IFMT) == S_IFDIR)
		return (126);
	return (0);
}

void	update_bin_path(t_shell *shell, t_token *token, char *path)
{
	free(token->key);
	token->key = ft_strdup(path);
	if (!token->key)
		exit_free(shell);
}

int	search_bin_path(t_shell *shell, t_token *token)
{
	int			i;
	int			ret;
	char		**path_tab;

	if (!token->key[0])
		return (127);
	ret = check_bin_path(token->key);
	if (!ret || ft_strchr(token->key, '/'))
		return (ret);
	path_tab = build_pathtab(shell, token->key);
	if (!path_tab)
		return (127);
	i = -1;
	while (path_tab[++i])
	{
		ret = check_bin_path(path_tab[i]);
		if (!ret || ret == 126)
		{
			if (!ret)
				update_bin_path(shell, token, path_tab[i]);
			break ;
		}
	}
	free_tab(path_tab);
	return (ret);
}

int	search_command(t_shell *shell)
{
	int	ret;

	if (is_builtin(pop_key(shell->current)))
		pop_token(shell->current)->symbol = T_BUILTIN;
	else
	{
		ret = search_bin_path(shell, pop_token(shell->current));
		if (ret)
			return (ret);
		pop_token(shell->current)->symbol = T_BIN;
	}
	return (0);
}
