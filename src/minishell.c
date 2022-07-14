/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/14 16:37:14 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_last_exit_status(t_shell *shell)
{
	char	*update;
	t_env	*tmp;

	update = ft_itoa(shell->ret);
	if (!update)
		return (0);
	tmp = get_env_ptr(shell, "?");
	free(tmp->str);
	tmp->str = ft_strjoin("?=", update);
	if (!tmp->str)
	{
		free(update);
		return (0);
	}
	free(update);
	return (1);
}

int	minishell(t_shell *shell)
{
	shell->ret = -1;
	shell->current = shell->start;
	sig.pid = 0;
	sig.signal = 0;
	while (parser(shell) == T_PIPE && shell->ret == 0)
		continue;
	if (sig.signal == 130)
		shell->ret = 1;
	if (shell->ret == 0)
		shell->ret = lexer(shell);
	if (shell->ret > 0 && sig.signal != 130)
		print_error(shell);
	update_history(shell, shell->history);
	// print_list(shell);
	if (shell->ret == 0)
		shell->ret = executor(shell, NULL);
	clear_parsing(shell);
	// ft_printf("%s######### MINISHELL ret = %2d ##########%s\n", CYAN, shell->ret, RESET);
	update_last_exit_status(shell);
	return (shell->ret);
}
