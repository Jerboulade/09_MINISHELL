/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:31:59 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/11 00:22:22 by jcarere          ###   ########.fr       */
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

char	*ft_strreverschr(const char *start, const char *current, char c)
{
	while (current > start)
	{
		if (*current == (char)c)
			return ((char *)current);
		current--;
	}
	if (*current == (char)c)
		return ((char *)current);
	return (NULL);
}

void	format_move_current_dir_first(char *path)
{
	while (19)
	{
		path = ft_strchr(path, '/');
		if (!path)
			break;
		while (ft_strncmp(path, "/./", 3) == 0)
			ft_strlcpy(path, path + 2, safe_strlen(path + 2) + 1);
		path++;
	}
}

void	format_shell_path(char *path)
{
	char	*start;
	char	*tmp;

	start = path;
	format_move_current_dir_first(path);
	while (19)
	{
		path = ft_strchr(path, '/');
		if (!path)
			break;
		while (ft_strncmp(path, "/../", 4) == 0)
		{
			ft_strlcpy(path, path + 3, safe_strlen(path + 3) + 1);
			if (path > start)
				tmp = ft_strreverschr(start, path - 1, '/');
			if (tmp)
				ft_strlcpy(tmp, path, safe_strlen(path) + 1);
			path = tmp;
			tmp = NULL;
		}
		path++;
	}
}

int	update_shell_path(t_shell *shell)
{
	char	*tmp;
	t_env	*update;

	update = get_env_ptr(shell, "SHELL");
	tmp = ft_strjoin("SHELL=", get_env(shell, "_"));
	format_shell_path(tmp);
	if (!tmp)
		return (0);
	if (update)
	{
		if (update->str)
			free(update->str);
		update->str = tmp;
	}
	else
	{
		update = new_env(tmp);
		if (!update)
			return (0);
		add_env(shell, update);
		free(tmp);
	}
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
	token->pos = -1; // try set other anywhere else (to spare one line)
	shell->start = ft_lstnew(token);
	if (!shell->start)
		exit_free(shell);
	shell->line = NULL;
	shell->senv = init_env(shell, env);
	if (!shell->senv)
		exit_free(shell);
	// ft_printf("%s################# ENV LIST #################\n", MAG);
	// t_env *tmp = shell->senv;
	// int i = 0;
	// while (tmp)
	// {
	// 	ft_printf("[%02d]%s\n", i++, tmp->str);
	// 	tmp = tmp->next;
	// }
	// ft_printf("%s\n", RESET);

	shell->history = init_history(shell);
	if (!shell->history)
		exit_free(shell);
	shell->parent = 1;
	// shell->end = 0;
	shell->fd_stdin = dup(STDIN_FILENO);
	shell->fd_stdout = dup(STDOUT_FILENO);
	// shell->fd_heredoc = -1;
	return (shell);
}
