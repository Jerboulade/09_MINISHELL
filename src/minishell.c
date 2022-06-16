/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 16:18:43 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_prompt(void)
{
	const char	*usr;
	const char	*host;
	const char	*pwd;

	usr = getenv("USER");
	host = getenv("HOSTNAME");
	// host = "190.1.1.1";
	// pwd = getenv("PWD") + ft_strlen(getenv("HOME"));
	pwd = ft_strrchr(getenv("PWD"), '/') + 1;
	// pwd = NULL;
	// pwd = getcwd((char *)pwd, MAXPATHLEN);
	if (usr)
		ft_printf("%s%s%s", MAG, usr, RESET);
	if (host)
		ft_printf("@%s%s%s", GREEN, host, RESET);
	if (pwd)
		ft_printf(":%s%s%s", CYAN, pwd, RESET);
	// ft_printf("\n");
	// free(pwd);
}

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->start = (t_list **)ft_calloc(1, sizeof(t_list *));
	if (!shell->start)
		return (NULL);
	shell->current = NULL;
	return (shell);
}

void	minishell(void)
{
	t_shell	*shell;
	t_parg	*parg;

	shell = init_shell();
	if (!shell)
		return ;
	parg = (t_parg *)ft_calloc(1, sizeof(t_parg));
	if (!parg)
		return ;
	display_prompt();
	parg->line = readline("> ");
	if (parg->line)
	{
		// ft_printf("%s#######################################\n", ORANGE);
		// ft_printf("                 PARSING                 \n");
		// ft_printf("#######################################%s\n", RESET);
		parg->pos = -1;
		parsing(shell, parg, 0);
		print_list(shell);
		ft_printf("\n%s########## MINISHELL ret = %d ##########%s\n", CYAN, parg->ret, RESET);
		free_parg(parg);
		// TO DO HERE : execute cmd line
		ft_printf("%s############# FREE SHELL ##############%s\n\n", CYAN, RESET);
		free_shell(shell);
		minishell();
	}
}
