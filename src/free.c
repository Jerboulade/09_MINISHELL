/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:41 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 20:53:20 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_parg(t_parg *parg)
{
	if (!parg)
		return ;
	if (parg->line)
		free(parg->line);
	free(parg);
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
	if (tmp->key)
		free(tmp->key);
	free(tmp);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (*shell->start)
		ft_lstclear(shell->start, &free_token);
	free_env_path(shell->env_path);
}
