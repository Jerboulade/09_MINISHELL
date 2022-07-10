/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 22:16:04 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/11 01:26:47 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_update(t_shell *shell, char *name, char *current_pwd)
{
	t_env	*update;
	char	*data;

	data = ft_strjoin(name, current_pwd);
	if (!data)
		exit_free(shell);
	update = new_env(data);
	if (!update)
		exit_free(shell);
	add_env(shell, update);
	free(data);
}

int	update_directory_path(t_shell *shell, char *name)
{
	t_env	*update;
	char	*current_pwd;

	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
		return (1);
	update = get_env_ptr(shell, name);
	name = ft_strjoin(name, "=");
	if (!name)
		exit_free(shell);
	if (!update)
		create_update(shell, name, current_pwd);
	else
	{
		free(update->str);
		update->str = ft_strjoin(name, current_pwd);
		if (!update->str)
			exit_free(shell);
	}
	free(name);
	free(current_pwd);
	return (0);
}

int	go_home(t_shell *shell)
{
	int		ret;
	char	*path;

	path = get_env(shell, "HOME");
	if (!path)
	{
		ft_dprintf(STDERR_FILENO, "minishell : cd : HOME not set\n");
		return (1);
	}
	if (update_directory_path(shell, "OLDPWD"))
		return (1);
	ret = chdir(path);
	if (ret != 0)
	{
		perror("minishell: cd : ");
		return (1);
	}
	if (update_directory_path(shell, "PWD"))
		return (1);
	return (0);
}

int	go_dir(t_shell *shell, char *path)
{
	int		ret;
	char	*tmp;

	tmp = NULL;
	if (path[0] == '-')
	{
		tmp = get_env(shell, "OLDPWD");
		if (!tmp)
			return (1);
		tmp = ft_strdup(tmp);
		if (!tmp)
			exit_free(shell);
		path = tmp;
	}
	update_directory_path(shell, "OLDPWD");
	ret = chdir(path);
	if (ret == -1)
	{
		perror("minishell : cd ");
		return (1);
	}
	update_directory_path(shell, "PWD");
	if (tmp)
		free(tmp);
	return (0);
}

int	msh_cd(t_shell *shell, char **av)
{
	// int	ret;

	// ret = 0;
	if (!av || !av[0] || !av[0][0])
		return (1);
	if (!av[1] && go_home(shell))
	// 	ret = go_home(shell);
	// if (ret)
		return (1);
	if (av[1] && av[1][0] == '-' && av[1][1])
	{
		ft_dprintf(STDERR_FILENO, "minishell : ");
		ft_dprintf(STDERR_FILENO, "cd : '%s' not a valid option\n", av[1]);
		return (1);
	}
	if (av[1] && go_dir(shell, av[1]))
		// ret = go_dir(shell, av[1]);
		return (1);
	return (0);
}
