/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 23:16:20 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/08 23:54:24 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_echo(t_shell *shell, char **av)
{
	int		i;
	char	c;

	(void)shell;
	if (!av || !av[0] || !*av[0])
		return (1);
	i = 1;
	c = '\n';
	if (av[i] && ft_strequ(av[1], "-n"))
	{
		c = 0;
		i++;
	}
	while (av[i])
	{
		ft_printf("%s", av[i]);
		if (av[++i])
			ft_printf(" ");
	}
	ft_printf("%c", c);
	return (0);
}
