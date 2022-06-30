/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/28 15:40:14 by jcarere          ###   ########.fr       */
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

# define CYAN "\033[1;36m"
# define GREEN "\033[1;32m"
# define MAG "\033[1;35m"
# define RED "\033[1;31m"
# define ORANGE "\033[1;33m"
# define RESET "\033[0m"
# define PROMPT_SIZE 100
# define HISTORY_SIZE 100
# define HISTORY_PATH "./datafile/history.log"
# define HEREDOC_PATH "./datafile/heredoc"
# define HEREDOC_FILE "./heredoc"
# define VARNAMESIZE_MAX 100

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
	T_START,
	T_EMPTY,
	T_WORD,
	T_PIPE,
	T_REDIRECT,
	T_REDIRIN,
	T_REDIROUT,
	T_HEREDOC,
	T_APPEND,
	T_BIN,
	T_BUILTIN,
	T_FILE
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
	int			index;
	char		*key;
	t_symbol	symbol;
}				t_token;

typedef struct s_hist
{
	char		**linetab;
	int			n;
}				t_hist;

typedef struct s_shell
{
	t_list		*start;
	t_list		*current;
	char		*line;
	char		**env;
	char		**env_path;
	int			ret;
	t_hist		*history;
	int			fd_redirect;
}				t_shell;
/*
** init.c
*/
int			rebuilt_envpath_string(char **env);
t_hist		*init_history(void);
char		**init_env_path(void);
t_shell		*init_shell(char **env);
/*
** init_utils.c
*/
/*
** minishell.c
*/
char		*display_prompt(int i, int j, char *tmp, char *miniprompt);
int			minishell(t_shell *shell);
/*
** parsing.c
*/
size_t	 	meta_token(t_shell *shell, int i);
char		*extract_key(t_shell *shell, int end);
int			parse_sequence(t_shell *shell, int *i);
int			parse_line(t_shell *shell);
t_symbol 	parser(t_shell *shell);
/*
** parsing_check.c
*/
int			token_is_redir(t_shell *shell);
int			last_token_is_meta(t_shell *shell);
int			is_empty_sequence(t_shell *shell, int i);
int			is_exception(t_shell *shell, char c);
/*
** tokenizer.c
*/
int			in_heredoc(int fd, char *key);
int			handle_heredoc(char *key);
int			tokenizer(t_shell *shell, char *key, int i);
/*
** expand.c
*/
char		*get_expanded_var(char **env, char *name, int len);
size_t		var_len(const char *var_start);
char		*get_expanded_key(t_shell *shell, char *key, char *newkey);
int			get_expanded_len(t_shell *shell, char *key);
char		*expand_key(t_shell *shell, char *key);
/*
** lexer.c
*/
int			is_existing_bin(t_shell *shell, t_token *token);
void		decrement_tokenpos(t_list *next);
int			is_builtin(char *key);
void		remove_quote(char *key);
int			is_dir(const char *key);
int			lexer(t_shell *shell);

/*
** token_utils.c
*/
t_symbol 	set_token_symbol(char *key);
int 		set_token_pos(t_shell *shell, t_token *token);
void 		token_push(t_shell *shell, t_token data);
t_token		*pop_token(t_list *node);
char		*pop_key(t_list *node);
t_symbol	pop_symbol(t_list *node);
int			pop_index(t_list *node);
int			pop_pos(t_list *node);
/*
** parsing_utils.c
*/
void 		join_newline(t_shell *shell, char *newline);
size_t		ft_skipcharlen(const char *str, char c);
int			is_end(char *line, int i);
int 		is_empty(const char *line);
int			is_whitespace(char c);
int			is_expandable(char *key);
int			is_start(char *line, int i);
void		set_quote(char *quote, char *line);
int	 		get_start_index(t_shell *shell);
/*
** print.c
*/
void		print_list(t_shell *shell);
int			print_parserror(t_shell *shell);
/*
** history.c
*/
void 		rebuilt_history_file(t_shell *shell, int fd);
void 		update_history(t_shell *shell, t_hist *hist);
void		fill_history(t_hist *history, int fd);
/*
** free.c
*/
int			exit_free(t_shell *shell);
void		free_env_path(char **env);
void		clear_parsing(t_shell *shell);
void		free_token(void *token);
void		free_shell(t_shell *shell);
#endif
