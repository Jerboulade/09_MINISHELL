/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/02 00:51:53 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*display_prompt(int i, int j, char *tmp, char *miniprompt)
{
	static char	prompt[PROMPT_SIZE + 33];

	tmp = getenv("USER");
	j += ft_strlcpy(prompt + j, MAG, 8);
	while (tmp && tmp[i] && j < PROMPT_SIZE + 7)
		prompt[j++] = tmp[i++];
	i = 0;
	tmp = getenv("HOSTNAME");
	j += ft_strlcpy(prompt + j, GREEN, 8);
	if (tmp)
		prompt[j++] = '@';
	while (tmp && tmp[i] && j < PROMPT_SIZE + 15)
		prompt[j++] = tmp[i++];
	i = 0;
	tmp = ft_strrchr(getenv("PWD"), '/') + 1;
	j += ft_strlcpy(prompt + j, CYAN, 8);
	if (tmp)
		prompt[j++] = ':';
	while (tmp && tmp[i] && j < PROMPT_SIZE + 23)
		prompt[j++] = tmp[i++];
	j += ft_strlcpy(prompt + j, RESET, 8);
	ft_strlcpy(prompt + j, "> ", 3);
	if (miniprompt)
		ft_strlcpy(prompt, miniprompt, ft_strlen(miniprompt) + 1);
	return (prompt);
}

int	minishell(t_shell *shell)
{
	shell->ret = -1;
	shell->current = shell->start;
	// ft_printf("%s#######################################\n", CYAN);
	// ft_printf("                 PARSER                  \n");
	// ft_printf("#######################################%s\n", RESET);
	while (parser(shell) == T_PIPE && shell->ret == 0)
		continue;
	update_history(shell, shell->history);
	if (shell->ret == 0)
		shell->ret = lexer(shell);
	if (shell->ret > 0)
		print_parserror(shell);
	print_list(shell);
	// !shell->ret ? ft_printf("%s############# EXECUTE LIST ############%s\n", MAG, RESET):-1;
	clear_parsing(shell);
	// ft_printf("%s########## MINISHELL ret = %2d #########%s\n", CYAN, shell->ret, RESET);
	return (shell->ret);
}
