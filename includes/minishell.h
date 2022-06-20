/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/20 01:21:07 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/lib_includes/libft.h"
# include <stdio.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/param.h>
# include <sys/stat.h>

# define CYAN "\033[0;36m"
# define GREEN "\033[0;32m"
# define MAG "\033[0;35m"
# define RED "\033[0;31m"
# define ORANGE "\033[0;33m"
# define RESET "\033[0m"
/*
** struct s_list prototype from libft
** ----------------------------------
**	typedef struct s_list
**	{
**		void			*data;
**		struct s_list	*next;
**	}					t_list;
*/
typedef enum e_symbol
{
	T_INVALID,
	T_START,
	T_COMMAND,
	T_BUILTIN,
	T_ARG,
	T_PIPE,
	T_REDIRECT,
	T_NO_SYM
}			t_symbol;

typedef struct s_parg
{
	char		*line;
	int			pos;
	int			ret;
	int			error_line;
	char		*error_file;

}				t_parg;

typedef struct s_token
{
	int			pos;
	char		*key;
	t_symbol	symbol;
}				t_token;

typedef struct s_shell
{
	t_list		*start;
	t_list		*current;
	char		*line;
	char		**env_path;
	int			ret;
	int			err_index;
}				t_shell;
/*
** minishell.c
*/
void		display_prompt(void);
int			rebuilt_path_string(char **env);
char		**init_env_path(void);
t_shell		*init_shell(void);
int			minishell(t_shell *shell);
/*
** parsing.c
*/
char		*red_key(t_shell *shell, int *i);
char		*arg_key(t_shell *shell, int start, int end);
int			get_token(t_shell *shell, int *pos, int *i, int j);
int			stop_parsing(t_shell *shell, int pos, int i);
int			parsing(t_shell *shell, int pos, int i);
/*
** tokenizer.c
*/
int			get_cmd_path(t_shell *shell, char **key);
t_symbol	get_token_symbol(t_shell *shell, int pos, char **key);
int			tokenizer(t_shell *shell, int pos, char *key);
/*
** parsing_utils.c
*/
size_t		ft_skipcharlen(const char *str, char c);
int			is_end(char *line, int i);
int			is_start(char *line, int i);
int			ret_pars(t_shell *shell, int i, int ret);
void		set_quote(char *quote, char *line);
/*
** print.c
*/
void		print_list(t_shell *shell);
int			print_parserror(t_shell *shell);
/*
** free.c
*/
int			exit_free(t_shell *shell);
void		free_env_path(char **env);
void		clear_parsing(t_shell *shell);
void		free_token(void *token);
void		free_shell(t_shell *shell);
#endif
