/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:33 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/02 16:27:10 by jcarere          ###   ########.fr       */
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

int	print_parserror(t_shell *shell)
{
	int	i;
	int	j;

	// ft_printf("\nIN PERROR PARSING\n");
	i = pop_index(shell->current);
	j = 0;
	if (shell->ret > 0)
	{
		ft_printf("%sminishell[%d]: ", RED, shell->ret);
		ft_printf("%ssyntax error\n", RESET);
	}
	ft_printf("%s%s\n", shell->line, GREEN);
	while (j < i || is_whitespace(shell->line[j]))
	{
		if (shell->line[j] >= 0 || (j % 2))
			ft_printf("~");
		j++;
	}
	ft_printf("%s^%s\n", RED, RESET);
	return (shell->ret);
}
