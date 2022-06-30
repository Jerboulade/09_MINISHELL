/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:31:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/28 18:11:00 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		rebuilt_envpath_string(char **env)
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

t_hist	*init_history(void)
{
	int		fd;
	t_hist	*history;

	history = ft_calloc(1, sizeof(*history));
	if (!history)
		return (NULL);
	history->linetab = ft_calloc(HISTORY_SIZE, sizeof(*history->linetab));
	if (!history->linetab)
		return (NULL);
	history->n = 0;
	fd = open(HISTORY_PATH, O_RDWR | O_CREAT, 0666);
	if (fd == -1)
	{
		ft_printf("%s[%s%s ", CYAN, HISTORY_PATH, RED);
		ft_printf("file corrupted.%s No history available]\n%s", CYAN, RESET);
		close(fd);
		return (history);
	}
	fill_history(history, fd);
	close(fd);
	return (history);
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
	if (!rebuilt_envpath_string(env))
	{
		free_env_path(env);
		return (NULL);
	}

	// ft_printf("%s############# ENV_PATH ################\n", MAG);
	// int i = -1;
	// while (env[++i])
	// 	printf("[%02d] %s\n", i + 1, env[i]);
	// ft_printf("%s\n", RESET);
	return (env);
}

t_shell	*init_shell(char **env)
{
	t_shell	*shell;
	t_token	*token;

	shell = ft_calloc(1, sizeof(*shell));
	if (!shell)
		return (NULL);
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		return (NULL);
	token->symbol = T_START;
	token->pos = -2;
	token->index = 0;
	shell->start = ft_lstnew(token);
	if (!shell->start)
		return (NULL);
	// shell->current = NULL;
	shell->line = NULL;
	// shell->ret = 0;
	shell->env_path = init_env_path();
	if (!shell->env_path)
		return (NULL);
	shell->env = env;
	shell->history = init_history();
	if (!shell->history)
		return (NULL);
	return (shell);
}
