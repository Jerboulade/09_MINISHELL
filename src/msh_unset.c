/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:22:23 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/11 00:22:55 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_node(t_shell *shell, char *arg)
{
	t_env	*node;

	node = get_env_ptr(shell, arg);
	if (node)
	{
		if (!node->prev)
		{
			shell->senv = node->next;
			shell->senv->prev = NULL;
		}
		else
		{
			node->prev->next = node->next;
			// ft_dprintf(shell->fd_stdout, "node = '%p'\n", node);
			if (node->next)
				node->next->prev = node->prev;
			// ft_dprintf(shell->fd_stdout, "node = '%p'\n", node);
		}
		free(node->str);
		free(node);
	}
}

int	proceed_unsetting(t_shell *shell, char **av)
{
	int		i;
	int		ret;
	int		exit_status;

	i = 0;
	exit_status = 0;
	while (av[++i])
	{
		// ft_dprintf(shell->fd_stdout, "arg = '%s'\n", av[i]);
		ret = check_arg_format(av[i]);
		// ft_dprintf(shell->fd_stdout, "arg = '%s'\n", av[i]);
		if (ret < 2 && ++exit_status)
			ft_dprintf(shell->fd_stdout, "minishell: unset: '%s' : not a valid indentifier\n", av[i]);
		else
			remove_node(shell, av[i]);
	}
	return (exit_status);
}

int	msh_unset(t_shell *shell, char **av)
{
	if (!av || !av[0])
		return (1);
	if (!av[1])
		return (0);
	if (av[1][0] == '-')
	{
		ft_dprintf(shell->fd_stdout, "minishell: export: '%s' : not a valid option\n", av[1]);
		return (1);
	}
	if (proceed_unsetting(shell, av))
		return (1);
	return (0);
}
