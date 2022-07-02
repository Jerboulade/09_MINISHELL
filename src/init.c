/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:31:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/02 01:48:53 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		append_backslash(char **env)
{
	int			i;
	char		*tmp;

	i = 0;
	while (env[i])
		i++;
	while (i--)
	{
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
	// char	*tmp;
	char	**env_path;

	// tmp = ft_strdup(getenv("PATH"));
	// if (!tmp)
	// 	return (NULL);
	// env = ft_split(tmp, ':');
	// if (!env)
	// {
	// 	free(tmp);
	// 	return (NULL);
	// }
	// free(tmp);
	env_path = ft_split(getenv("PATH"), ':');
	if (!env_path)
		return (NULL);
	if (!append_backslash(env_path))
		return (NULL);

	ft_printf("%s############# ENV_PATH ################\n", MAG);
	int i = -1;
	while (env_path[++i])
		printf("[%02d] %s\n", i + 1, env_path[i]);
	ft_printf("%s\n", RESET);

	return (env_path);
}

char	**init_env(char **env)
{
	int		len;
	char	**env_copy;

	len = 0;
	while (env[len])
		len++;
	env_copy = ft_calloc(len + 1, sizeof(*env_copy));
	if (!env_copy)
		return (NULL);
	env_copy[len] = NULL;
	while (len--)
	{
		env_copy[len] = ft_strdup(env[len]);
		if (!env_copy[len])
			return (NULL);
	}
	ft_printf("%s################# ENV #################\n", MAG);
	int i = -1;
	while (env_copy[++i])
		printf("[%02d] %s\n", i + 1, env_copy[i]);
	ft_printf("%s\n", RESET);

	return (env_copy);
}

t_shell	*init_shell(char **env)
{
	t_shell	*shell;
	t_token	*token;

	shell = ft_calloc(1, sizeof(*shell));
	if (!shell)
		exit_free(shell);
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		exit_free(shell);
	token->symbol = T_START;
	token->pos = -1; // try set other anywhere else (to spare one line)
	// token->index = 0; // check if calloc set this to zero and remove in case (to spare one line)
	shell->start = ft_lstnew(token);
	if (!shell->start)
		exit_free(shell);
	// shell->current = NULL;
	shell->line = NULL;
	shell->env = init_env(env);
	if (!shell->env)
		exit_free(shell);
	shell->env_path = init_env_path();
	if (!shell->env_path)
		exit_free(shell);
	shell->history = init_history();
	if (!shell->history)
		exit_free(shell);
	return (shell);
}
