/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/06 23:13:54 by jcarere          ###   ########.fr       */
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
	// ft_dprintf(shell->fd_stdout, "errno after parser = %d\n", errno);
	if (shell->ret == 0)
		shell->ret = lexer(shell);
	if (shell->ret > 0)
		print_error(shell);
	// ft_dprintf(shell->fd_stdout, "errno after lexer= %d\n", errno);
	update_history(shell, shell->history);
	// print_list(shell);
	// ft_dprintf(shell->fd_stdout, "errno update history = %d\n", errno);

	// !shell->ret ? ft_printf("%s############# EXECUTE LIST ############%s\n", MAG, RESET):-1;
	if (shell->ret == 0)
		shell->ret = executor(shell);
	// ft_printf("coucou pid %d\n", getpid());
	if (shell->ret > 0)
		print_error(shell);
	// ft_printf("coucou pid %d\n", getpid());
	clear_parsing(shell);
	// waitpid(0, &shell->ret, 0);
	// shell->ret = WEXITSTATUS(shell->ret);
	// ft_printf("%s########## MINISHELL ret = %2d #########%s\n", CYAN, shell->ret, RESET);

	return (shell->ret);
}
