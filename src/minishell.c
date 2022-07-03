/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/03 22:28:12 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell(t_shell *shell)
{
	shell->ret = -1;
	shell->current = shell->start;
	// ft_printf("%s#######################################\n", CYAN);
	// ft_printf("                 PARSER                  \n");
	// ft_printf("#######################################%s\n", RESET);
	while (parser(shell) == T_PIPE && shell->ret == 0)
		continue;
	if (shell->ret == 0)
		shell->ret = lexer(shell);
	if (shell->ret > 0)
		print_parserror(shell);
	update_history(shell, shell->history);
	print_list(shell);
	// !shell->ret ? ft_printf("%s############# EXECUTE LIST ############%s\n", MAG, RESET):-1;
	if (shell->ret == 0)
		shell->ret = executor(shell);
	clear_parsing(shell);
	// ft_printf("%s########## MINISHELL ret = %2d #########%s\n", CYAN, shell->ret, RESET);
	return (shell->ret);
}
