/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:33 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/20 01:21:40 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_shell *shell)
{
	t_list		*tmp;
	t_token		*token;
	const char	*symbol;

	tmp = shell->start;
	ft_printf("%s\n#######################################\n", ORANGE);
	ft_printf("               TOKEN LIST              \n");
	ft_printf("#######################################\n");
	ft_printf("<POS|%-20.20s*|%-11s>%s\n", "KEY", "T_SYMBOL", RESET);
	while (tmp)
	{
		token = (t_token *)tmp->data;
		ft_printf("%s<%s%03d%s|%s", ORANGE, RESET, token->pos, ORANGE, RESET);
		ft_printf("%-20.20s*%s|%s", token->key, ORANGE, RESET);
		if (token->symbol == T_COMMAND)
			symbol = "T_COMMAND";
		else if (token->symbol == T_BUILTIN)
			symbol = "T_BUILTIN";
		else if (token->symbol == T_ARG)
			symbol = "T_ARG";
		else if (token->symbol == T_PIPE)
			symbol = "T_PIPE";
		else if (token->symbol == T_REDIRECT)
			symbol = "T_REDIRECT";
		else if (token->symbol == T_INVALID)
			symbol = "T_INVALID";
		else if (token->symbol == T_START)
			symbol = "T_START";
		ft_printf("%-11s%s>%s\n", symbol, ORANGE, RESET);
		tmp = tmp->next;
	}
	ft_printf("%s#######################################%s\n", ORANGE, RESET);
}

int	print_parserror(t_shell *shell)
{
	int	i;
	int	j;

	// ft_printf("\nIN PERROR PARSING\n");
	// ft_printf("PARG: ret[%d] pos[%d]\n", parg->ret, parg->pos);
	i = shell->err_index;
	j = -1;
	ft_printf("%sminishell(%d):", RED, shell->ret);
	ft_printf("%ssyntax error\n", RESET);
	ft_printf("%s%s\n", shell->line, GREEN);
	while (++j < i)
	{
		if (shell->line[j] >= 0 || (j % 2))
			ft_printf("~");
	}
	ft_printf("%s^%s\n", RED, RESET);
	return (shell->ret);
}
