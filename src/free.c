/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:41 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/23 19:16:22 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_free(t_shell *shell)
{
	write(STDERR_FILENO, "exit : malloc error\n", 20);
	if (shell)
		free_shell(shell);
	exit(EXIT_FAILURE);
}

void	clear_parsing(t_shell *shell)
{
	if (!shell)
		return ;
	// if (shell->line)
	// 	free(shell->line);
	shell->line = NULL;
	if (shell->start && shell->start->next)
		ft_lstclear(&shell->start->next, &free_token);
	// ft_printf("\nIN CLEAR PARSING start->next = %p\n", shell->start->next);
}

void	free_env_path(char **env)
{
	int	i;

	if (!env)
		return ;
	i = -1;
	while (env[++i])
		free(env[i]);
	free(env);
}

void	free_token(void *token)
{
	t_token	*tmp;

	if (!token)
		return ;
	tmp = (t_token *)token;
	ft_printf("%s############# FREE TOKEN ############## %s%s\n", CYAN, tmp->key, RESET);
	if (tmp->key)
		free(tmp->key);
	free(tmp);
}

void	free_history(t_hist *history)
{
	int	i;

	i = -1;
	if (!history)
		return ;
	while (history->linetab && ++i < history->n)
	{
		if (history->linetab[i])
			free(history->linetab[i]);
	}
	if (history->linetab)
		free(history->linetab);
	free(history);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->start)
		ft_lstclear(&shell->start, &free_token);
	// ft_printf("free %s\n", shell->line);
	free_history(shell->history);
	// ft_printf("free %p\n", shell->start);
	// free(shell->start);
	free_env_path(shell->env_path);
	free(shell);
}
