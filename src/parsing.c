/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:36 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/14 17:42:42 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_key(t_shell *shell, int end)
{
	int		start;
	char	*key;
	size_t	len;

	// ft_printf("\nIN EXTRACT KEY \n");
	start = get_start_index(shell);
	len = end - start;
	key = ft_calloc(len + 1, sizeof(*key));
	if (!key)
		exit_free(shell);
	ft_strlcpy(key, shell->line + start, len + 1);
	return (key);
}

int	parse_sequence(t_shell *shell, int *i)
{
	int ret;

	// ft_printf("\nIN PARSE SEQUENCE \n");
	// ft_printf("c = %c\n", shell->line[*i]);
	if (!is_start(shell->line, *i) && !is_empty_sequence(shell, *i)) /*!shell->line[*i] ||  !ft_strchr("|><", shell->line[*i])*/
	{
		ret = tokenizer(shell, extract_key(shell, *i), *i);
		if (ret)
			return (ret);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	if (shell->line[*i] && ft_strchr("|><", shell->line[*i]))
	{
		if (shell->line[*i] == '|' && pop_symbol(shell->current) == T_START)
			return (2);
		if (is_meta(pop_symbol(shell->current)) && !is_exception(shell, shell->line[*i]))
			return (3);
		*i += meta_token(shell, *i);
	}
	*i += ft_skipcharlen(shell->line + *i, ' ');
	if (!shell->line[*i] && is_redir(pop_symbol(shell->current)))
		return (4);
	return (0);
}

int	parse_line(t_shell *shell)
{
	int		i;
	int		ret;
	char	quote;

	// ft_printf("\n%s############ IN PARSE LINE ############\n", GREEN, RESET);
	// print_error(shell);
	quote = 0;
	i = get_start_index(shell);
	while (1)
	{
		// ft_printf("%s###char =  %c%s\n", GREEN, shell->line[i], RESET);
		if ((!shell->line[i] || ft_strchr("|> <", shell->line[i])) && !quote)
		{
			ret = parse_sequence(shell, &i);
			if (ret != 0 || !shell->line[i])
				return (ret);
		}
		else if (ft_strchr("\'\"", shell->line[i]))
		{
			set_quote(&quote, shell->line + i);
			i++;
		}
		else
			i++;
	}
	return (0);
}

char	*display_prompt(int i, int j, char *tmp, t_shell *shell)
{
	static char	prompt[PROMPT_SIZE + 42];

	tmp = get_env(shell, "USER");
	j += ft_strlcpy(prompt + j, MAG, 10);
	while (tmp && tmp[i] && j < PROMPT_SIZE + 9)
		prompt[j++] = tmp[i++];
	i = 0;
	tmp = get_env(shell, "HOSTNAME");
	j += ft_strlcpy(prompt + j, GREEN, 10);
	if (tmp)
		prompt[j++] = '@';
	while (tmp && tmp[i] && j < PROMPT_SIZE + 19)
		prompt[j++] = tmp[i++];
	i = 0;
	tmp = get_env(shell, "PWD");
	if (tmp)
		tmp = ft_strrchr(tmp, '/') + (ft_strrchr(tmp, '/') != NULL);
	j += ft_strlcpy(prompt + j, CYAN, 10);
	if (tmp)
		prompt[j++] = ':';
	while (tmp && tmp[i] && j < PROMPT_SIZE + 29)
		prompt[j++] = tmp[i++];
	j += ft_strlcpy(prompt + j, RESET, 10);
	ft_strlcpy(prompt + j, "> ", 3);
	return (prompt);
}

void	handle_signal(int signum, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signum == SIGINT)
	{
		if (!sig.pid)
			close(STDIN_FILENO);
		else
		{
			waitpid(0, NULL, 0);
			rl_redisplay();
		}
		sig.signal = 130;
	}
	else if (signum == SIGQUIT)
	{
		if (!sig.pid)
			rl_redisplay();
		else
			waitpid(0, NULL, 0);
		sig.signal = 131;
	}
}

t_symbol parser(t_shell *shell)
{
	char *newline;

	if (!shell->line)
		shell->line = readline(display_prompt(0, 0, NULL, shell));
	else
	{
		// sig.sub = 1;
		newline = readline("pipe> ");
		// printf( "coucou2-%d", sig.pid);
		if (!newline)
			return (-1);
		else if (is_empty(newline))
		{
			// printf( "coucou3-%d", sig.pid);
			free(newline);
			return (T_PIPE);
		}
		// ft_printf("is not empty, [%s]\n", newline);
		join_newline(shell, newline);
	}
	// ft_printf("#######################################\n");
	if (!shell->line)				// case : ctrl d
		shell->ret = -1;
	else if (is_empty(shell->line))
		shell->ret = 0;				// case : no command
	else
		shell->ret = parse_line(shell);
	return (pop_symbol(shell->current));
}
