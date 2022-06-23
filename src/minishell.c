/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:24:39 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/23 02:52:29 by jcarere          ###   ########.fr       */
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

int		rebuilt_path_string(char **env)
{
	int			i;
	char		*tmp;

	i = 0;
	while (env[i])
		i++;
	while (--i >= 0)
	{
		if (env[i][0] != '/')
		{
			tmp = ft_strjoin("/", env[i]);
			if (!tmp)
				return (0);
			free(env[i]);
			env[i] = tmp;
		}
		if (env[i][ft_strlen(env[i]) - 1] != '/')
		{
			tmp = ft_strjoin(env[i], "/");
			if (!tmp)
				return (0);
			free(env[i]);
			env[i] = tmp;
		}
	}
	return (1);
}

char	**init_env_path()
{
	char	*tmp;
	char	**env;

	tmp = ft_strdup(getenv("PATH"));
	if (!tmp)
		return (NULL);
	env = ft_split(tmp, ':');
	if (!env)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	if (!rebuilt_path_string(env))
	{
		free_env_path(env);
		return (NULL);
	}

	ft_printf("%s############# ENV_PATH ################\n", MAG);
	int i = -1;
	while (env[++i])
		printf("[%02d] %s\n", i + 1, env[i]);
	ft_printf("%s\n", RESET);
	return (env);
}

t_shell	*init_shell(void)
{
	t_shell	*shell;
	t_token	*token;

	shell = ft_calloc(1, sizeof(*shell));
	if (!shell)
		return (NULL);
	token = ft_calloc(1, sizeof(*token));
	if (!token)
		return (NULL);
	token->symbol = T_START;
	token->pos = -2;
	token->index = 0;
	shell->start = ft_lstnew(token);
	if (!shell->start)
		return (NULL);
	// shell->current = NULL;
	shell->line = NULL;
	// shell->ret = 0;
	shell->env_path = init_env_path();
	if (!shell->env_path)
		return (NULL);

	return (shell);
}

void join_newline(t_shell *shell, char *newline)
{
	char *tmp;

	tmp = ft_strjoin(shell->line, newline);
	if (!tmp)
		exit_free(shell);
	free(newline);
	free(shell->line);
	shell->line = tmp;
}

t_symbol parser(t_shell *shell)
{
	char *newline;

	if (!shell->line)
		shell->line = readline(display_prompt(0, 0, NULL, NULL));
	else
	{
		newline = readline(display_prompt(0, 0, NULL, "pipe> "));
		if (!newline)
			return (-1);
		else if (is_empty(newline))
		{
			free(newline);
			return (T_PIPE);
		}
		join_newline(shell, newline);
	}
	if (!shell->line)
		shell->ret = -1;
	else if (is_empty(shell->line))
		shell->ret = 0;
	else
		shell->ret = parsing(shell);
	return (pop_symbol(shell->current));
}

int	minishell(t_shell *shell)
{
	shell->ret = -1;
	shell->current = shell->start;
	ft_printf("%s#######################################\n", CYAN);
	ft_printf("                 PARSER                  \n");
	ft_printf("#######################################%s\n", RESET);
	while (parser(shell) == T_PIPE && shell->ret == 0)
		continue;
	add_history(shell->line);
	print_list(shell); // exec_line();
	if (shell->ret > 0)
		print_parserror(shell);
	ft_printf("%s#######################################\n", CYAN);
	ft_printf("                  LEXER                  \n");
	ft_printf("#######################################%s\n", RESET);
	!shell->ret ? ft_printf("%s############# EXECUTE LIST ############%s\n", MAG, RESET):-1;
	clear_parsing(shell);
	ft_printf("%s########## MINISHELL ret = %2d #########%s\n", CYAN, shell->ret, RESET);
	return (shell->ret);
}
