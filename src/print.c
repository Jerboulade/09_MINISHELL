/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:33 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/14 03:26:03 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*print_symbol(t_symbol symbol)
{
	if (symbol == T_BIN)
		return ("T_BIN");
	else if (symbol == T_BUILTIN)
		return ("T_BUILTIN");
	else if (symbol == T_NEWLINE)
		return ("T_NEWLINE");
	else if (symbol == T_PIPE)
		return ("T_PIPE");
	else if (symbol == T_REDIRIN)
		return ("T_REDIRIN");
	else if (symbol == T_REDIROUT)
		return ("T_REDIROUT");
	else if (symbol == T_APPEND)
		return ("T_APPEND");
	else if (symbol == T_HEREDOC)
		return ("T_HEREDOC");
	else if (symbol == T_WORD)
		return ("T_WORD");
	return ("T_START");
}

void	print_list(t_shell *shell)
{
	t_list		*tmp;
	t_token		*token;
	const char	*symbol;

	tmp = shell->start;
	ft_printf("%s#######################################\n", ORANGE);
	ft_printf("               TOKEN LIST              \n");
	ft_printf("#######################################\n");
	ft_printf("<POS|%-20.20s*|%-11s> i%s\n", "KEY", "T_SYMBOL", RESET);
	while (tmp)
	{
		token = (t_token *)tmp->data;
		ft_printf("%s<%s%03d%s|%s", ORANGE, RESET, token->pos, ORANGE, RESET);
		ft_printf("%-20.20s*%s|%s", token->key, ORANGE, RESET);
		symbol = print_symbol(token->symbol);
		ft_printf("%-11s%s> %s%d\n", symbol, ORANGE, RESET, token->index);
		tmp = tmp->next;
	}
	ft_printf("%s#######################################%s\n", ORANGE, RESET);
}

int	print_errno(char *object, int ret)
{
	ft_dprintf(STDERR_FILENO, "%sminishell:%s %s: ", RED, RESET, object);
	perror("");
	return (ret);
}

int	print_error(t_shell *shell)
{
	int	i;
	int	j;

	// ft_printf("\nIN PERROR PARSING\n");
	// if (errno)
	// 	print_errno(shell);
	i = pop_index(shell->current);
	j = -1;
	ft_printf("%sminishell:%s ", RED, RESET);
	if (shell->ret < 6)
		ft_printf("syntax error\n");
	else if (shell->ret == 126)
		ft_printf("%s: is a directory\n", pop_key(shell->current));
	else if (shell->ret == 127)
		ft_printf("%s: command not found\n", pop_key(shell->current));
	else
	{
		ft_printf("%s%s\n", shell->line, GREEN);
		while (++j < i)
		{
			if (shell->line[j] >= 0 || (j % 2))
				ft_printf("~");
		}
		ft_printf("%s^%s\n", RED, RESET);
	}
	return (shell->ret);
}
