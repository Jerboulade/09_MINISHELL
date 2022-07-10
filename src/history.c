/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 04:02:21 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/11 00:08:10 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void rebuilt_history_file(t_shell *shell, int fd)
{
	int i;

	i = 0;
	rl_clear_history();
	free(shell->history->linetab[0]);
	while (++i < HISTORY_SIZE)
		shell->history->linetab[i - 1] = shell->history->linetab[i];
	shell->history->linetab[HISTORY_SIZE - 1] = NULL;
	shell->history->n = 0;
	while(shell->history->n < HISTORY_SIZE - 1)
	{
		ft_dprintf(fd, "%s\n", shell->history->linetab[shell->history->n]);
		add_history(shell->history->linetab[shell->history->n]);
		shell->history->n++;
	}
}

void update_history(t_shell *shell, t_hist *hist)
{
	int fd;

	if (!shell->line)
		return ;
	if (!*shell->line  || \
		(hist->n && ft_strequ(shell->line, hist->linetab[hist->n - 1])))
	{
		free(shell->line);
		return ;
	}
	if (hist->n == HISTORY_SIZE)
		fd = open(hist->path, O_WRONLY | O_TRUNC);
	else
		fd = open(hist->path, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		free(shell->line);
		return ;
	}
	if (hist->n == HISTORY_SIZE)
		rebuilt_history_file(shell, fd);
	ft_dprintf(fd, "%s\n", shell->line);
	hist->linetab[hist->n++] = shell->line;
	add_history(shell->line);
	close(fd);
}

void	fill_history(t_hist *history, int fd)
{
	char	*tmp;

	while (history->n < HISTORY_SIZE)
	{
		tmp = NULL;
		history->linetab[history->n] = get_next_line(fd);
		if (!history->linetab[history->n])
			break;
		tmp = ft_strchr(history->linetab[history->n], '\n');
		if (tmp)
			*tmp = 0;
		add_history(history->linetab[history->n]);
		history->n++;
	}
}
