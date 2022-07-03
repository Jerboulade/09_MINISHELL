/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:08 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/03 02:46:14 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void head(char **env)
{
	t_shell	*shell;

	shell = init_shell(env);
	while (minishell(shell) > -1)
		continue;
	free_shell(shell);
}

int	main(int ac, char **av, char **env)
{

	(void)ac;
	(void)av;
	// env;
	head(env);
	// ########## TEST : MULTI PROCESS ##########
	// pid_t pid;
	// const char *whoami = "parent";

	// pid = fork();
	// if (!pid)
	// 	whoami = "child";
	// else
	// 	wait(NULL);
	// ft_printf("\n%s [%i]\n\n", whoami, (int)pid);
	// ft_printf("ENV NAME          : getenv(\"NAME\") //-> (look at \'$> env\' for available NAME)\n");
	// ft_printf("USER              : %s\n", getenv("USER"));
	// ft_printf("PWD               : %s\n", getenv("PWD"));
	// ft_printf("OLDPWD            : %s\n", getenv("OLDPWD"));
	// ft_printf("SECURITYSESSIONID : %s\n", getenv("SECURITYSESSIONID"));
	// ft_printf("HOST               : %s\n", getenv("HOST "));
	// ft_printf("PATH              : %s\n", getenv("PATH"));

	// ########## TEST : PRINT ENVIRONNEMENT ##########
	// size_t len = 0;
	// while (env[len])
	// {
	// 	ft_printf("%s\n", env[len]);
	// 	len++;
	// }

	// ########## CHECK LEAKS ##########
	system("leaks minishell");
	// while (1);
	return (0);
}
