/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 13:33:03 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/17 20:32:48 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_arg_format(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '=')
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	if (arg[i] == '=')
		return (1);
	return (2);
}

void	update_value(t_shell *shell, char *arg, char *name, char **av)
{
	t_env	*tmp;
	char	*updated;

	tmp = get_env_ptr(shell, name);
	updated = ft_strdup(arg);
	if (!updated)
	{
		free(name);
		free(av);
		exit_free(shell);
	}
	free(tmp->str);
	tmp->str = updated;
	free(name);
}

void	create_value(t_shell *shell, char *arg, char *name, char **av)
{
	t_env	*new;

	new = new_env(arg);
	if (!new)
	{
		free(name);
		free(av);
		exit_free(shell);
	}
	add_env(shell, new);
	free(name);
}

void	proceed_exportation(t_shell *shell, char **av)
{
	int		i;
	int		ret;
	char	*name;

	i = 0;
	name = NULL;
	while (av[++i])
	{
		ret = check_arg_format(av[i]);
		if (!ret)
		{
			ft_dprintf(STDERR_FILENO, "%sminishell:%s ", RED, RESET);
			ft_dprintf(STDERR_FILENO, "export: invalid arguments\n");
		}
		else if (ret == 1)
		{
			name = dup_env_varname(shell, av[i]);
			if (get_env(shell, name))
				update_value(shell, av[i], name, av);
			else
				create_value(shell, av[i], name, av);
		}
	}
}

int	msh_export(t_shell *shell, char **av)
{
	if (!av || !av[0])
		return (1);
	if (!av[1])
		return (0);
	if (av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%sminishell:%s ", RED, RESET);
		ft_dprintf(STDERR_FILENO, "export: '%s': not a valid option\n", av[1]);
		return (1);
	}
	proceed_exportation(shell, av);
	return (0);
}
