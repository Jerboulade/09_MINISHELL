/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:33 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/04 22:32:00 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (token->symbol == T_BIN)
			symbol = "T_BIN";
		else if (token->symbol == T_BUILTIN)
			symbol = "T_BUILTIN";
		else if (token->symbol == T_NEWLINE)
			symbol = "T_NEWLINE";
		else if (token->symbol == T_PIPE)
			symbol = "T_PIPE";
		else if (token->symbol == T_REDIRIN)
			symbol = "T_REDIRIN";
		else if (token->symbol == T_REDIROUT)
			symbol = "T_REDIROUT";
		// else if (token->symbol == T_REDIRECT)
		// 	symbol = "T_REDIRECT";
		else if (token->symbol == T_APPEND)
			symbol = "T_APPEND";
		else if (token->symbol == T_HEREDOC)
			symbol = "T_HEREDOC";
		else if (token->symbol == T_FILE)
			symbol = "T_FILE";
		else if (token->symbol == T_START)
			symbol = "T_START";
		else if (token->symbol == T_WORD)
			symbol = "T_WORD";
		else if (token->symbol == T_EMPTY)
			symbol = "T_EMPTY";
		ft_printf("%-11s%s> %s%d\n", symbol, ORANGE, RESET, token->index);
		tmp = tmp->next;
	}
	ft_printf("%s#######################################%s\n", ORANGE, RESET);
}

int	print_errno(t_shell *shell)
{
	ft_printf("%sminishell[errno:%d]:%s ", RED, errno, RESET);
	ft_printf("\'%s\' : ", pop_key(shell->current));
	perror("");
	return (errno);
}

int	print_error(t_shell *shell)
{
	int	i;
	int	j;

	// ft_printf("\nIN PERROR PARSING\n");
	if (errno)
		return (print_errno(shell));
	i = pop_index(shell->current);
	j = -1;
	ft_printf("%sminishell[%d]:%s ", RED, shell->ret, RESET);
	if (shell->ret > 0 && shell->ret < 6)
		ft_printf("syntax error\n");
	else if (shell->ret == 6)
		ft_printf("\'%s\' : is a directory\n", pop_key(shell->current));
	else if (shell->ret == 7)
		ft_printf("\'%s\' : command not found\n", pop_key(shell->current));
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
