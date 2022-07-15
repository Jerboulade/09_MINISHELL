/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 01:12:40 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:15:27 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_selected_for_execution(t_shell *shell, char **tab)
{
	return ((!shell->current || pop_pos(shell->current) == 0) \
			&& (!shell->parent || shell->end) \
			&& !shell->ret \
			&& tab);
}

int	is_flag(t_symbol symbol)
{
	return (symbol == T_PIPE || symbol == T_NEWLINE);
}

int	is_redir(t_symbol symbol)
{
	return (symbol == T_REDIRIN \
			|| symbol == T_REDIROUT \
			|| symbol == T_APPEND \
			|| symbol == T_HEREDOC);
}

int	is_meta(t_symbol symbol)
{
	return (symbol == T_REDIRIN \
			|| symbol == T_REDIROUT \
			|| symbol == T_REDIRIN \
			|| symbol == T_HEREDOC \
			|| symbol == T_PIPE);
}

int	is_builtin(char *key)
{
	return ((ft_strequ(key, "echo") || ft_strequ(key, "cd") \
		|| ft_strequ(key, "pwd") || ft_strequ(key, "export") \
		|| ft_strequ(key, "unset") || ft_strequ(key, "env") \
		|| ft_strequ(key, "exit")));
}
