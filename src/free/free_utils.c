/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:20:46 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:16:49 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc(t_shell *shell)
{
	t_hdoc	*tmp;

	while (shell->heredoc)
	{
		tmp = shell->heredoc;
		shell->heredoc = shell->heredoc->next;
		free(tmp);
	}
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
	if (history->path)
		free(history->path);
	free(history);
	rl_clear_history();
}
