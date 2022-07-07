/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:31:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/07 22:10:46 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	update_shlvl(t_shell *shell)
{
	int		shlvl;
	char	*update;
	t_env	*tmp;

	shlvl = ft_atoi(get_env(shell, "SHLVL")) + 1;
	update = ft_itoa(shlvl);
	if (!update)
		return (0);
	tmp = get_env_ptr(shell, "SHLVL");
	free(tmp->str);
	tmp->str = ft_strjoin("SHLVL=", update);
	if (!tmp->str)
	{
		free(update);
		return (0);
	}
	free(update);
	return (1);
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
	token->pos = -1; // try set other anywhere else (to spare one line)
	shell->start = ft_lstnew(token);
	if (!shell->start)
		exit_free(shell);
	shell->line = NULL;
	shell->senv = init_env(shell, env);
	if (!shell->senv)
		exit_free(shell);
	ft_printf("%s################# ENV LIST #################\n", MAG);
	t_env *tmp = shell->senv;
	int i = 0;
	while (tmp)
	{
		ft_printf("[%02d]%s\n", i++, tmp->str);
		tmp = tmp->next;
	}
	ft_printf("%s\n", RESET);

	shell->history = init_history();
	if (!shell->history)
		exit_free(shell);
	shell->parent = 1;
	// shell->end = 0;
	shell->fd_stdin = dup(STDIN_FILENO);
	shell->fd_stdout = dup(STDOUT_FILENO);
	// shell->fd_heredoc = -1;
	return (shell);
}
