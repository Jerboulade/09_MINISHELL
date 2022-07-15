/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:11:08 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 14:06:09 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_process(t_shell *shell, int fd[2], pid_t pid)
{
	if (pid == 0)
	{
		shell->parent = 0;
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		waitpid(pid, &shell->ret, 0);
		if (shell->ret >= 256)
			shell->ret /= 256;
		if (g_sig.signal == 131)//
			ft_printf("Quit: %d\n", g_sig.signal - 128);//
		if (g_sig.signal)//
			shell->ret = g_sig.signal;//
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
}

int	open_pipe(t_shell *shell)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (print_errno("pipe", 1));
	pid = fork();
	if (pid == -1)
		return (print_errno("fork", 1));
	pipe_process(shell, fd, pid);
	return (shell->ret);
}

int	handle_pipe(t_shell *shell, t_symbol exec_type, char **tab)
{
	int	fd[2];

	if (exec_type == T_BIN)
	{
		shell->ret = open_pipe(shell);
		(void)tab;
	}
	else if (exec_type == T_BUILTIN)
	{
		if (pipe(fd) == -1)
			shell->ret = print_errno("pipe", 1);
		if (!shell->ret && dup2(fd[1], STDOUT_FILENO) == -1)
			shell->ret = print_errno("dup2", 1);
		close(fd[1]);
		shell->end = 1;
	}
	return (fd[0]);
}
