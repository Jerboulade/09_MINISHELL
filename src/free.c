/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:41 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/04 22:33:33 by jcarere          ###   ########.fr       */
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

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

void	free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*tmp_next;

	tmp = *env;
	while (tmp)
	{
		tmp_next = tmp->next;
		free(tmp->str);
		free(tmp);
		tmp = tmp_next;
	}
	env = NULL;
}

void	free_token(void *token)
{
	t_token	*tmp;

	if (!token)
		return ;
	tmp = (t_token *)token;
	// ft_printf("%s############# FREE TOKEN ############## %s%s\n", CYAN, tmp->key, RESET);
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
	free_env(&shell->senv);
	// ft_printf("free %p\n", shell->start);
	// free(shell->start);
	// free_tab(shell->env);
	// free_tab(shell->env_path);
	free(shell);
}
