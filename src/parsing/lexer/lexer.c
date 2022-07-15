/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 21:01:48 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:14:22 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_backslash(char **path_tab)
{
	int			i;
	char		*tmp;

	i = 0;
	while (path_tab[i])
		i++;
	while (i--)
	{
		if (path_tab[i][ft_strlen(path_tab[i]) - 1] != '/')
		{
			tmp = ft_strjoin(path_tab[i], "/");
			if (!tmp)
				return (0);
			free(path_tab[i]);
			path_tab[i] = tmp;
		}
	}
	return (1);
}

t_list	*find_nextflag(t_shell *shell)
{
	t_list	*tmp;
	int		moveflag;

	tmp = shell->current;
	while (pop_symbol(tmp) != T_PIPE && pop_symbol(tmp) != T_NEWLINE)
		tmp = tmp->next;
	moveflag = pop_index(tmp);
	while (tmp->next && pop_index(tmp->next) < moveflag)
		tmp = tmp->next;
	return (tmp);
}

void	merge_and_move(t_shell *shell, t_list *tmp_prev, int moveflag)
{
	t_list	*tmp_next;
	t_list	*tmp_curr;

	if (pop_key(shell->current) == NULL)
	{
		tmp_next = shell->current->next;
		pop_token(shell->current)->key = pop_key(tmp_next);
		pop_token(tmp_next)->key = NULL;
		shell->current->next = shell->current->next->next;
		ft_lstdelone(tmp_next, &free_token);
	}
	if (!moveflag || (moveflag && pop_index(shell->current) > moveflag))
	{
		tmp_next = find_nextflag(shell);
		tmp_curr = shell->current;
		tmp_prev->next = tmp_curr->next;
		shell->current = tmp_prev;
		tmp_prev = tmp_next;
		tmp_next = tmp_prev->next;
		tmp_prev->next = tmp_curr;
		tmp_curr->next = tmp_next;
	}
}

int	lexer(t_shell *shell)
{
	t_list	*tmp_prev;
	int		moveflag;
	int		ret;

	moveflag = 0;
	if (is_redir(pop_symbol(shell->current)))
		return (5);
	token_add_newline(shell);
	shell->current = shell->start;
	while (pop_symbol(shell->current) != T_NEWLINE)
	{
		if (pop_symbol(shell->current) == T_WORD && !pop_pos(shell->current))
		{
			ret = search_command(shell);
			if (ret)
				return (ret);
		}
		else if (is_redir(pop_symbol(shell->current)))
			merge_and_move(shell, tmp_prev, moveflag);
		else if (is_flag(pop_symbol(shell->current)))
			moveflag = pop_index(shell->current);
		tmp_prev = shell->current;
		shell->current = shell->current->next;
	}
	return (0);
}
