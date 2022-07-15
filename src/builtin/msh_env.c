/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 23:33:07 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/10 17:51:13 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_env(t_shell *shell, char **av)
{
	t_env	*tmp;

	if (!av || !av[0] || !*av[0] || !shell->senv)
		return (1);
	tmp = shell->senv->next;
	while (tmp)
	{
		ft_printf("%s\n", tmp->str);
		tmp = tmp->next;
	}
	return (0);
}
