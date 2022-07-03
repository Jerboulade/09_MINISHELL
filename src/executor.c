/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 18:20:40 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/03 22:53:32 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(char **tab)
{
	int i;

	i = -1;
	ft_printf("execute [");
	while (tab[++i])
		ft_printf(" %s", tab[i]);
	ft_printf(" ]\n");
	free(tab);
	return (0);
}

char	**get_cmd_tab(t_shell *shell)
{
	int		i;
	int		len;
	t_list	*tmp;
	char	**tab;

	len = 1;
	tmp = shell->current;
	while (pop_pos(tmp) < pop_pos(tmp->next))
	{
		len++;
		tmp = tmp->next;
	}
	tab = ft_calloc(len + 1, sizeof(char *));
	if (!tab)
		exit_free(shell);
	tab[len] = NULL;
	tmp = shell->current;
	i = -1;
	while (++i < len)
	{
		tab[i] = pop_key(tmp);
		tmp = tmp->next;
	}
	return (tab);
}

int	executor(t_shell *shell)
{
	char		**tab;
	t_symbol	symbol;

	tab = NULL;
	ft_printf("\n%s############ IN EXECUTOR ############\n", GREEN, RESET);
	shell->current = shell->start;
	while (shell->current)
	{
		symbol = pop_symbol(shell->current);
		if (is_redir(symbol))
			ft_printf("do redirection in [ %s ]\n", pop_key(shell->current));
		else if (symbol == T_PIPE)
			ft_printf("open pipe\n");
		else if (pop_pos(shell->current) == 0)
			tab = get_cmd_tab(shell);
		shell->current = shell->current->next;
		if ((!shell->current || pop_pos(shell->current) == 0) && tab)
		{
			execute(tab);
			tab = NULL;
		}
	}
	return (0);
}
