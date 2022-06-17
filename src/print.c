/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:33 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 18:51:20 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_shell *shell)
{
	t_list		*tmp;
	t_token		*token;
	const char	*symbol;

	tmp = *shell->start;
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
		else if (token->symbol == T_NO_SYM)
			symbol = "T_NO_SYM";
		ft_printf("%-11s%s>%s\n", symbol, ORANGE, RESET);
		tmp = tmp->next;
	}
	ft_printf("%s#######################################%s\n", ORANGE, RESET);
}

int	print_parserror(t_parg *parg, int i)
{
	int	j;

	// ft_printf("\nIN PERROR PARSING\n");
	// ft_printf("PARG: ret[%d] pos[%d]\n", parg->ret, parg->pos);
	j = -1;
	if (!parg->ret)
		return (0);
	ft_printf("%sminishell:error[%d]:", RED, parg->ret);
	ft_printf("%s:line%d: ", parg->error_file, parg->error_line);
	if (parg->ret > 0)
	{
		ft_printf("%ssyntax error\n", RESET);
		ft_printf("%s%s\n", parg->line, GREEN);
		while (++j < i)
		{
			if (parg->line[j] >= 0 || (j % 2))
				ft_printf("~");
		}
		ft_printf("%s^%s\n", RED, RESET);
	}
	else
		ft_printf("%smalloc error\n", RESET);
	return (parg->ret);
}
