/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:08 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 13:31:39 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	head(char **env)
{
	int					ret;
	t_shell				*shell;
	struct sigaction	sa_params;

	sa_params.sa_handler = NULL;
	sa_params.sa_flags = SA_SIGINFO;
	sa_params.sa_sigaction = handle_signal;
	sigaction(SIGINT, &sa_params, NULL);
	sigaction(SIGQUIT, &sa_params, NULL);
	rl_catch_signals = 0;
	shell = init_shell(env);
	while (minishell(shell) > -1 && !shell->exit)
		continue ;
	if (shell->exit == -1)
		shell->ret = 0;
	else if (shell->exit)
		shell->ret = shell->exit;
	ret = shell->ret;
	free_shell(shell);
	return (ret);
}

int	main(int ac, char **av, char **env)
{
	int	ret;

	(void)ac;
	(void)av;
	ret = head(env);
	return (ret);
}
