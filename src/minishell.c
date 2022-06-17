/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/17 03:34:35 by jcarere          ###   ########.fr       */
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

int		rebuilt_path_string(char **env)
{
	int			i;
	char		*tmp;

	i = 0;
	while (env[i])
		i++;
	while (--i >= 0)
	{
		if (env[i][0] != '/')
		{
			tmp = ft_strjoin("/", env[i]);
			if (!tmp)
				return (0);
			free(env[i]);
			env[i] = tmp;
		}
		if (env[i][ft_strlen(env[i]) - 1] != '/')
		{
			tmp = ft_strjoin(env[i], "/");
			if (!tmp)
				return (0);
			free(env[i]);
			env[i] = tmp;
		}
	}
	return (1);
}

char	**init_env_path()
{
	char	*tmp;
	char	**env;

	tmp = ft_strdup(getenv("PATH"));
	if (!tmp)
		return (NULL);
	env = ft_split(tmp, ':');
	if (!env)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	if (!rebuilt_path_string(env))
	{
		free_env_path(env);
		return (NULL);
	}

	ft_printf("%s############# ENV_PATH ################\n", MAG);
	int i = -1;
	while (env[++i])
		printf("[%02d] %s\n", i + 1, env[i]);
	ft_printf("\n");

	return (env);
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
	shell->env_path = init_env_path();
	if (!shell->env_path)
		return (NULL);
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
