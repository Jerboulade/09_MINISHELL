/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 22:44:14 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/14 23:34:25 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_last_exit_status(t_shell *shell)
{
	char	*update;
	t_env	*tmp;

	update = ft_itoa(shell->ret);
	if (!update)
		exit_free(shell);
	tmp = get_env_ptr(shell, "?");
	free(tmp->str);
	tmp->str = ft_strjoin("?=", update);
	if (!tmp->str)
	{
		free(update);
		exit_free(shell);
	}
	free(update);
}

void	update_last_command(t_shell *shell, char *cmd)
{
	char	*tmp;
	t_env	*update;

	update = get_env_ptr(shell, "_");
	tmp = ft_strjoin("_=", cmd);
	if (!tmp)
		exit_free(shell);
	if (update)
	{
		if (update->str)
			free(update->str);
		update->str = tmp;
	}
	else
	{
		update = new_env(tmp);
		if (!update)
			exit_free(shell);
		add_env(shell, update);
		free(tmp);
	}
}

int	update_shlvl(t_shell *shell)
{
	int		shlvl;
	char	*update;
	t_env	*tmp;

	shlvl = ft_atoi(get_env(shell, "SHLVL")) + 1;
	update = ft_itoa(shlvl);
	if (!update)
		return (0);
	tmp = get_env_ptr(shell, "SHLVL");
	free(tmp->str);
	tmp->str = ft_strjoin("SHLVL=", update);
	if (!tmp->str)
	{
		free(update);
		return (0);
	}
	free(update);
	return (1);
}

int	update_shell_path(t_shell *shell)
{
	char	*tmp;
	t_env	*update;

	update = get_env_ptr(shell, "SHELL");
	tmp = ft_strjoin("SHELL=", get_env(shell, "_"));
	format_shell_path(tmp);
	if (!tmp)
		return (0);
	if (update)
	{
		if (update->str)
			free(update->str);
		update->str = tmp;
	}
	else
	{
		update = new_env(tmp);
		if (!update)
			return (0);
		add_env(shell, update);
		free(tmp);
	}
	return (1);
}
