/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:08 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 03:18:02 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int perror_parsing(char *line, int pos, char *file, int err)
{
	int errli;
	int ret;

	if (err < 0)
	{
		err *= -1;
		ret = -1;
	}
	else
		ret = err % 1000;
	errli = err / 1000;
	if (!ret)
		return (0);
	ft_printf("%sminishell:error[%d]:%s:line%d: ", RED, ret, file, errli);
	if (ret > 0)
	{
		ft_printf("%ssyntax error\n", RESET);
		ft_printf("'%s'%s\n ", line, GREEN);
		while (pos--)
			ft_printf("~");
		ft_printf("%s^%s\n", RED, RESET);
	}
	else
		ft_printf("%smalloc error\n", RESET);
	return (errli);
}

void print_list(t_shell *shell)
{
	t_list *tmp;
	t_token *token;
	const char *symbol;

	tmp = *shell->start;
	ft_printf("%s\n#######################################\n", ORANGE);
	ft_printf("               TOKEN LIST              \n");
	ft_printf("#######################################\n");
	ft_printf(" <POS, %19s, %10s>%s\n", "KEY", "T_SYMBOL", RESET);
	while (tmp)
	{
		token = (t_token *)tmp->data;
		ft_printf(" <%03d, %19s,", token->pos, token->key);
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
		else if (token->symbol == T_TEST)
			symbol = "T_TEST";
		ft_printf(" %10s>\n", symbol);
		tmp = tmp->next;
	}
	ft_printf("%s#######################################%s\n", ORANGE, RESET);
}

void	display_prompt(void)
{
	const char *usr;
	const char *host;
	const char *pwd;

	usr = getenv("USER");
	host = getenv("HOSTNAME");
	// host = "190.1.1.1";
	// pwd = getenv("PWD") + ft_strlen(getenv("HOME"));
	pwd = ft_strrchr(getenv("PWD"), '/') + 1;
	// pwd = NULL;
	// pwd = getcwd((char *)pwd, MAXPATHLEN);
	if (usr)
		ft_printf("%s%s%s", MAG, usr, RESET);
	if (host)
		ft_printf("@%s%s%s", GREEN, host, RESET);
	if (pwd)
		ft_printf(":%s%s%s", CYAN, pwd, RESET);
	// ft_printf("\n");
	// free(pwd);
}

void	set_quote(char *quote, char *line)
{
	// ft_printf("\nIN SET QUOTE\n");
	// ft_printf("quote = [%c]\nc = [%c]\n", *quote, *line);
	if (!*quote && ft_strchr(line + 1, *line))
		*quote = *line;
	else if (*quote == *line)
		*quote = 0;
	// ft_printf("new quote = [%c]\nc = [%c]\n", *quote, *line);
}

int	tokenizer(t_shell *shell, char *key, int pos)
{
	t_token	*token;

	ft_printf("\nIN TOKENIZER\n");
	if (!key || !*key)
		return (0);
	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return (0);
	token->key = key; // token->key = meta_interpretation;
	token->pos = pos;

	token->symbol = T_TEST;// token->symbol = get_token_symbol(token);
	if (!pos)
		token->symbol = T_COMMAND;
	// ft_printf("key = %s\n", key);
	if (!*shell->start)
	{
		*shell->start = ft_lstnew(token);
		if (!*shell->start)
			return (0);
		shell->current = *shell->start;
	}
	else
	{
		shell->current->next = ft_lstnew(token);
		if (!shell->current->next)
			return (0);
		shell->current = shell->current->next;
	}
	return (1);
}
// PARSING TEST TRAP
//touch test"file" -> only 1 file called 'testfile' created (same for simple quote)
char	*arg_key(char *start, char *end)
{
	char	*key;
	size_t	len;
	// size_t	ret;

	ft_printf("\nIN GET KEY\n");

	len = end - start;
	// ft_printf("len = %d - start = %s\n", len, start);
	key = (char *)ft_calloc(len + 1, sizeof(char));
	if (!key)
		return (NULL);
	ft_strlcpy(key, start, len + 1);
	// if (len == ft_strlcpy(key, start, len)) ---> cannot work because strlcpy(dest, src, len) returns src_len
	// 	return (key);
	// free(key);
	ft_printf("key = [%s]\n", key);
	return (key);
}
// char *get_key()

size_t	ft_skipcharlen(const char *str, char c)
{
	char *tmp;

	if (!str)
		return (0);
	tmp = (char *)str;
	while (*str && *tmp == c)
		tmp++;
	return (tmp - str);
}

char	*red_key(char *line, int *i, int *pos)
{
	char	*key;

	ft_printf("\nIN RED KEY\n");

	key = NULL;
	*pos = -1;
	if (line[*i] == '|')
		key = (ft_strdup("| "));
	else if (line[*i] == '>' && line[*i + 1] != '>')
		key = (ft_strdup("> "));
	else if (line[*i] == '>' && line[*i + 1] == '>')
		key = (ft_strdup(">>"));
	else if (line[*i] == '<' && line[*i + 1] != '<')
		key = (ft_strdup("< "));
	else if (line[*i] == '<' && line[*i + 1] == '<')
		key =  (ft_strdup("<<"));
	if (!key)
		return (NULL);
	*i += ft_skipcharlen(key, key[0]);
	return (key);
}

int is_end(char *line, int i)
{
	if (!line[i])
		return (1);
	while (line[++i])
	{
		if (line[i] != ' ')
			return (0);
	}
	return (1);
}

int is_start(char *line, int i)
{
	if (!i)
		return (1);
	while (--i)
	{
		if (line[i] != ' ')
			return (0);
	}
	if (line[i] != ' ')
		return (0);
	return (1);
}
//BIG TEST #######################################
int stop_parsing(char *line, t_shell *shell, int i, int pos)
{
	t_token *prev;

	if (!is_start(line, i))
		prev = (t_token *)shell->current->data;
	if (!line[i] && is_end(line, i))
		return (1);
	else if (!line[i] && pos == -1 && prev->key[0] != '|')
		return (1);
	else if (!line[i])
		return (1);
	else if (is_start(line, i) && ft_strchr("|><", line[i]) && is_end(line, i))
		return (1);
	else if (is_start(line, i) && line[i] == '|')
		return (1);
	else if (ft_strchr("|><", line[i]) && pos == -1 && !is_start(line, i))
		return (1);
	return (0);
}

int stop_val(char *line, t_shell *shell, int i, int pos)
{
	t_token *prev;

	ft_printf("\nIN STOP VAL\n");
	ft_printf("char in stop_val = [%c]\n", line[i]);
	if (!is_start(line, i))
		prev = (t_token *)shell->current->data;
	if (!line[i] && is_start(line, i))
		return (0);
	else if (!line[i] && pos == -1 && prev->key[0] != '|')
		return (perror_parsing(line, i, __FILE__, __LINE__ * 1000 + 1));
	else if (!line[i])
		return (0);
	else if (is_start(line, i) && ft_strchr("|><", line[i]) && is_end(line, i))
		return (perror_parsing(line, i, __FILE__, __LINE__ * 1000 + 2));
	else if (is_start(line, i) && line[i] == '|')
		return (perror_parsing(line, i, __FILE__, __LINE__ * 1000 + 3));
	else if (ft_strchr("|><", line[i]) && pos == -1 && !is_start(line, i))
		return (perror_parsing(line, i, __FILE__, __LINE__ * 1000 + 4));
	return (0);
}

int split_arg(char *line, t_shell *shell, int i, int pos)
{
	char	quote;
	int		j;

	ft_printf("\n%s########### IN SPLIT ARG %03d ##########%s\n", GREEN, pos, RESET);
	// ft_printf("starting char = [%c]\n", line[i]);
	quote = 0;
	i += ft_skipcharlen(line + i, ' ');
	if (stop_parsing(line, shell, i, pos))
		return (stop_val(line, shell, i, pos));
	j = i;
	// ft_printf("starting char2 = [%c]\n", line[i]);
	while (line[i])
	{
		// ft_printf("%c, i = %d, j = %d, quote = %c\n", line[j], i, j, quote);
		if (ft_strchr("|> <", line[i]) && !quote)
			break ;
		else if (ft_strchr("\'\"", line[i]))
			set_quote(&quote, line + i);
		i++;
	}
	// ft_printf("char before tokenizer ' ' = [%c]\n", line[i])
	if (!is_start(line, i))
	{
		if (!tokenizer(shell, arg_key(line + j, line + i), ++pos))
			return (perror_parsing(line, i, __FILE__, __LINE__ * -1000));
	}
	i += ft_skipcharlen(line + i, ' ');
	if (line[i] && ft_strchr("|><", line[i]))
	{
		if (!tokenizer(shell, red_key(line, &i, &pos), pos))
			return (perror_parsing(line, i, __FILE__, __LINE__ * -1000));
	}
	return (split_arg(line, shell, i, pos));
}

void	free_token(void *token)
{
	t_token *tmp;

	tmp = (t_token *)token;
	if (tmp->key)
		free(tmp->key);
	free(tmp);
}

void	free_shell(t_shell *shell)
{
	if (*shell->start)
		ft_lstclear(shell->start, &free_token);
}

t_shell *init_shell(void)
{
	t_shell *shell;

	shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->start = (t_list **)ft_calloc(1, sizeof(t_list *));
	if (!shell->start)
		return (NULL);
	shell->current = NULL;
	return (shell);
}

void minishell(void)
{
	char *line;
	t_shell *shell;

	shell = init_shell();
	if (!shell)
		return ;
	display_prompt();
	line = readline("> ");
	if (line)
	{
		// ft_printf("%s", ORANGE);
		// ft_printf("#######################################\n");
		// ft_printf("                 PARSING                 \n");
		// ft_printf("#######################################\n");
		// ft_printf("%s", RESET);
		split_arg(line, shell, 0, -1);
		free(line);
		print_list(shell);
		// here : execute cmd line
		// ft_printf("\n%s############## FREE SHELL ##############%s\n", CYAN, RESET);
		free_shell(shell);
		minishell();
	}
}

int	main(int ac, char **av, char **env)
{
	// t_shell *shell;
	// size_t len = 0;
	// const char *whoami = "parent";
	// pid_t pid;
	// char *test;

	(void)ac;
	(void)av;
	(void)env;
	// write(1, "\033[1;96mcoucou\033[0m", 23);

	//TEST readline + get command line.
	// shell = init_shell();
	// if (!shell)
	// 	return (-1);
	minishell();
	// printf("%s\n", "IN FREE SHELL");
	// free_shell(shell);


	// TEST MULTI PROCESS
	// pid = fork();
	// if (!pid)
	// 	whoami = "child";
	// else
	// 	wait(NULL);
	// ft_printf("\n%s [%i]\n\n", whoami, (int)pid);
	// ft_printf("ENV NAME          : getenv(\"NAME\") //-> (look at \'$> env\' for available NAME)\n");
	// ft_printf("USER              : %s\n", getenv("USER"));
	// ft_printf("PWD               : %s\n", getenv("PWD"));
	// ft_printf("OLDPWD            : %s\n", getenv("OLDPWD"));
	// ft_printf("SECURITYSESSIONID : %s\n", getenv("SECURITYSESSIONID"));
	// ft_printf("HOST               : %s\n", getenv("HOST "));
	// ft_printf("PATH              : %s\n", getenv("PATH"));


	// len = 0;
	// while (env[len])
	// {
	// 	ft_printf("%s\n", env[len]);
	// 	len++;
	// }
	// system("leaks minishell");
	// sleep(1);
	// while (1);
	return (0);
}
