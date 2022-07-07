/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/08 00:13:06 by jcarere          ###   ########.fr       */
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

# define CYAN "\001\033[1;36m\002"
# define GREEN "\001\033[1;32m\002"
# define MAG "\001\033[1;35m\002"
# define RED "\001\033[1;31m\002"
# define ORANGE "\001\033[1;33m\002"
# define RESET "\001\033[0m\002"
# define PROMPT_SIZE 100
# define HISTORY_SIZE 100
# define HISTORY_PATH "./datafile/history.log"
# define VARNAMESIZE_MAX 100

typedef enum e_symbol
{
	T_START,
	T_EMPTY,
	T_WORD,
	T_PIPE,
	T_NEWLINE,
	T_REDIRIN,
	T_REDIROUT,
	T_HEREDOC,
	T_APPEND,
	T_BIN,
	T_BUILTIN,
	T_FILE
}			t_symbol;
/*
** struct s_list prototype from libft
** ----------------------------------
**	typedef struct s_list
**	{
**		void			*data;
**		struct s_list	*next;
**	}					t_list;
*/
typedef struct s_env
{
	char			*str;
	struct s_env	*next;
	struct s_env	*prev;
}					t_env;

typedef struct s_hdoc
{
	int				fd;
	struct s_hdoc	*next;
}					t_hdoc;

typedef struct s_token
{
	int				pos;
	int				index;
	char			*key;
	t_symbol		symbol;
}					t_token;

typedef struct s_hist
{
	char			**linetab;
	int				n;
}					t_hist;

typedef struct s_shell
{
	t_list			*start;
	t_list			*current;
	char			*line;
	t_env			*senv;
	t_hdoc			*heredoc;
	int				ret;
	int				parent;
	int				end;
	int				fd_stdin;
	int				fd_stdout;
	// int				fd_heredoc[2];
	t_hist			*history;
	int				fd_redirect;
}					t_shell;
/*
** init.c
*/
t_hist		*init_history(void);
t_env		*init_env(t_shell *shell, char **env);
t_shell		*init_shell(char **env);
/*
** env_utils.c
*/
t_env		*new_env(char *data);
void		add_env(t_shell *shell, t_env *new_env);
t_env		*get_env_ptr(t_shell *shell, char *name);
char		*get_env(t_shell *shell, char *name);
/*
** minishell.c
*/
int			minishell(t_shell *shell);
/*
** parsing.c
*/
char		*extract_key(t_shell *shell, int end);
int			parse_sequence(t_shell *shell, int *i);
int			parse_line(t_shell *shell);
char		*display_prompt(int i, int j, char *tmp, t_shell *shell);
t_symbol 	parser(t_shell *shell);
/*
** parsing_utils.c
*/
void		remove_quote(char *key);
void 		join_newline(t_shell *shell, char *newline);
size_t		ft_skipcharlen(const char *str, char c);
void		set_quote(char *quote, char *line);
int	 		get_start_index(t_shell *shell);
/*
** tokenizer.c
*/
void		token_add_newline(t_shell *shell);
int			in_heredoc(t_shell *shell, char *key, int fd);
int			handle_heredoc(t_shell *shell, char *key);
size_t	 	meta_token(t_shell *shell, int i);
int			tokenizer(t_shell *shell, char *key, int i);
/*
** token_utils.c
*/
int 		set_token_pos(t_shell *shell, t_token *token);
void 		token_push(t_shell *shell, t_token data);
/*
** token_pop.c
*/
t_token		*pop_token(t_list *node);
char		*pop_key(t_list *node);
t_symbol	pop_symbol(t_list *node);
int			pop_index(t_list *node);
int			pop_pos(t_list *node);
/*
** expand.c
*/
char		*get_expanded_key(t_shell *shell, char *key, char *newkey);
char		*get_varname(t_shell *shell, const char *key);
size_t		safe_strlen(const char *str);
int			get_expanded_len(t_shell *shell, char *key);
char		*expand_key(t_shell *shell, char *key);
/*
** lexer.c
*/

int			append_backslash(char **env);
t_list		*find_nextflag(t_shell *shell);
void		merge_and_move(t_shell *shell, t_list *tmp_prev, int moveflag);
int			file_check(t_shell *shell);
char		**build_pathtab(char *paths, char *key);
int			search_relbin(t_shell *shell, t_token *token);
int			search_absbin(t_shell *shell, t_token *token);
int			search_command(t_shell *shell);
int			lexer(t_shell *shell);
/*
** executor.c
*/

int			exec_bin(t_shell *shell, char **tab);
char		**get_cmd_tab(t_shell *shell);
int			executor(t_shell *shell);
/*
** is_check.c
*/
int			is_flag(t_symbol symbol);
int			is_redir(t_symbol symbol);
int			is_meta(t_symbol symbol);
int			is_empty_sequence(t_shell *shell, int i);
int			is_exception(t_shell *shell, char c);
int			is_relbin(char *key);
int			is_builtin(char *key);
int 		is_empty(const char *line);
int			is_whitespace(char c);
int			is_expandable(char *key);
int			is_start(char *line, int i);
/*
** print.c
*/
void		print_list(t_shell *shell);
int			print_errno(t_shell *shell);
int			print_error(t_shell *shell);
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
int			exit_success(t_shell *shell);
void		free_tab(char **tab);
void		clear_parsing(t_shell *shell);
void		free_token(void *token);
void		free_shell(t_shell *shell);
/*
** msh_pwd.c
*/
int			ft_pwd(t_shell *shell);

#endif
