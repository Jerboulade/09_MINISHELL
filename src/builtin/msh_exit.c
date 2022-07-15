/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 17:44:48 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/14 20:02:20 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strisdigit(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	msh_exit(t_shell *shell, char **av)
{
	if (shell->piped)
		return (0);
	shell->exit = 1;
	ft_dprintf(STDERR_FILENO, "exit\n");
	if (av[1] && av[2])
	{
		shell->exit = 1;
		ft_dprintf(STDERR_FILENO, "%sminishell:%s ", RED, RESET);
		ft_dprintf(STDERR_FILENO, "exit: too many arguments\n");
	}
	else if (av[1] && !ft_strisdigit(av[1]))
	{
		shell->exit = 255;
		ft_dprintf(STDERR_FILENO, "%sminishell:%s ", RED, RESET);
		ft_dprintf(STDERR_FILENO, "exit: numeric argument required\n");
	}
	else if (av[1])
	{
		shell->exit = ft_atoi(av[1]) % 256;
		if (!shell->exit)
			shell->exit = -1;
	}
	else
		shell->exit = -1;
	return (1);
}
