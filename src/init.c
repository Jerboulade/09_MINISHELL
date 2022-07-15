/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:31:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 00:01:48 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_history_path(t_shell *shell, t_hist *history)
{
	char	*shell_path;
	char	*tmp;

	shell_path = ft_strdup(get_env(shell, "SHELL"));
	if (!shell_path)
		return (0);
	tmp = ft_strrchr(shell_path, '/');
	*(tmp + 1) = 0;
	history->path = ft_strjoin(shell_path, HISTORY_PATH);
	free(shell_path);
	if (!history->path)
		return (0);
	return (1);
}

t_hist	*init_history(t_shell *shell)
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
	if (!init_history_path(shell, history))
		return (NULL);
	fd = open(history->path, O_RDWR | O_CREAT, 0666);
	if (fd == -1)
	{
		ft_printf("%s[%s%s ", CYAN, shell->history->path, RED);
		ft_printf("file corrupted.%s No history available]\n%s", CYAN, RESET);
		close(fd);
		return (history);
	}
	fill_history(history, fd);
	close(fd);
	return (history);
}

t_env	*init_env(t_shell *shell, char **env)
{
	int		i;
	t_env	*new;

	i = -1;
	shell->senv = new_env("?=0");
	while (env[++i])
	{
		new = new_env(env[i]);
		if (!new)
			return (NULL);
		add_env(shell, new);
	}
	if (!update_shlvl(shell))
		return (NULL);
	if (!update_shell_path(shell))
		return (NULL);
	return (shell->senv);
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
	token->pos = -1;
	shell->start = ft_lstnew(token);
	if (!shell->start)
		exit_free(shell);
	shell->line = NULL;
	shell->senv = init_env(shell, env);
	if (!shell->senv)
		exit_free(shell);
	shell->history = init_history(shell);
	if (!shell->history)
		exit_free(shell);
	shell->parent = 1;
	shell->fd_stdin = dup(STDIN_FILENO);
	shell->fd_stdout = dup(STDOUT_FILENO);
	return (shell);
}
