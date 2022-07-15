/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 04:32:56 by jcarere          ###   ########.fr       */
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
# include <signal.h>

# define CYAN "\001\033[1;36m\002"
# define GREEN "\001\033[1;32m\002"
# define MAG "\001\033[1;35m\002"
# define RED "\001\033[1;31m\002"
# define ORANGE "\001\033[1;33m\002"
# define RESET "\001\033[0m\002"
# define PROMPT_SIZE 100
# define HISTORY_SIZE 100
# define HISTORY_PATH "./data/history.log"
# define VARNAMESIZE_MAX 100

typedef enum e_symbol
{
	T_START,
	T_WORD,
	T_PIPE,
	T_NEWLINE,
	T_REDIRIN,
	T_REDIROUT,
	T_HEREDOC,
	T_APPEND,
	T_BIN,
	T_BUILTIN,
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

typedef struct s_sig
{
	int				pid;
	int				signal;
}					t_sig;

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
	char			*path;
	int				n;
}					t_hist;

typedef struct s_shell
{
	char			*line;
	t_list			*start;
	t_list			*current;
	t_env			*senv;
	t_hdoc			*heredoc;
	t_hist			*history;
	char			piped;
	char			parent;
	int				fd_stdin;
	int				fd_stdout;
	int				end;
	int				exit;
	int				fd_redirect;
	int				ret;
}					t_shell;

/*
** main.c
*/
int			main(int ac, char **av, char **env);
int			head(char **env);
/*
** init.c
*/
int			init_history_path(t_shell *shell, t_hist *history);
t_hist		*init_history(t_shell *shell);
t_env		*init_env(t_shell *shell, char **env);
t_shell		*init_shell(char **env);
/*
** env_update.c
*/
void		update_last_exit_status(t_shell *shell);
void		update_last_command(t_shell *shell, char *cmd);
int			update_shlvl(t_shell *shell);
int			update_shell_path(t_shell *shell);
/*
** env_utils.c
*/
char		*dup_env_varname(t_shell *shell, char *arg);
t_env		*new_env(char *data);
void		add_env(t_shell *shell, t_env *new_env);
t_env		*get_env_ptr(t_shell *shell, char *name);
char		*get_env(t_shell *shell, char *name);
/*
** env_utils2.c
*/
void		format_move_current_dir_first(char *path);
void		format_shell_path(char *path);
char		**env_listoar(t_shell *shell);
/*
** minishell.c
*/
void		handle_signal(int signum, siginfo_t *info, void *context);
int			minishell(t_shell *shell);
/*
** parsing.c
*/
char		*extract_key(t_shell *shell, int end);
int			parse_sequence(t_shell *shell, int *i);
int			parse_line(t_shell *shell);
char		*display_prompt(int i, int j, char *tmp, t_shell *shell);
t_symbol	parser(t_shell *shell);
/*
** parsing_utils.c
*/
char		*ft_strreverschr(const char *start, const char *current, char c);
void		join_newline(t_shell *shell, char *newline);
size_t		ft_skipcharlen(const char *str, char c);
void		set_quote(char *quote, char *line);
int			get_start_index(t_shell *shell);
/*
** tokenizer.c
*/
void		token_add_newline(t_shell *shell);
int			in_heredoc(t_shell *shell, char *key, int fd);
int			handle_heredoc(t_shell *shell, char *key);
size_t		meta_token(t_shell *shell, int i);
int			tokenizer(t_shell *shell, char *key, int i);
/*
** token_utils.c
*/
int			set_token_pos(t_shell *shell, t_token *token);
void		token_push(t_shell *shell, t_token data);
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
int			replace_newkey(t_shell *shell, int *i, char *newkey, char **key);
char		*get_expanded_key(t_shell *shell, char *key, char *newkey);
int			get_expanded_len(t_shell *shell, char *key);
void		remove_quote(char *key);
char		*expand_key(t_shell *shell, char *key);
/*
** expand_utils.c
*/
char		*get_varname(t_shell *shell, const char *key);
size_t		safe_strlen(const char *str);
int			compute_newlen(t_shell *shell, int *i, char **key);
/*
** lexer.c
*/
int			append_backslash(char **path_tab);
t_list		*find_nextflag(t_shell *shell);
void		merge_and_move(t_shell *shell, t_list *tmp_prev, int moveflag);
int			lexer(t_shell *shell);
/*
** lexer_path_utils.c
*/
char		**build_pathtab(t_shell *shell, char *key);
int			check_bin_path(char *path);
void		update_bin_path(t_shell *shell, t_token *token, char *path);
int			search_bin_path(t_shell *shell, t_token *token);
int			search_command(t_shell *shell);
/*
** executor.c
*/
char		**get_cmd_tab(t_shell *shell);
int			exec_builtin(t_shell *shell, char **tab);
int			exec_bin(t_shell *shell, char **tab, char **env_array);
void		execute(t_shell *shell, t_symbol type, char **tab, int fdin_pipe);
int			executor(t_shell *shell, char **tab);
/*
** handle_redir.c
*/
void		redir_in(t_shell *shell, t_symbol symbol);
void		redir_out(t_shell *shell, t_symbol symbol);
void		handle_redir(t_shell *shell, t_symbol symbol, char **tab);
/*
** handle_pipe.c
*/
void		pipe_process(t_shell *shell, int fd[2], pid_t pid);
int			open_pipe(t_shell *shell);
int			handle_pipe(t_shell *shell, t_symbol exec_type, char **tab);
/*
** is_check.c
*/
int			is_selected_for_execution(t_shell *shell, char **tab);
int			is_flag(t_symbol symbol);
int			is_redir(t_symbol symbol);
int			is_meta(t_symbol symbol);
int			is_builtin(char *key);
/*
** is_check2.c
*/
int			is_expandable(char *key);
int			is_whitespace(char c);
int			is_start(char *line, int i);
int			is_empty_sequence(t_shell *shell, int i);
int			is_exception(t_shell *shell, char c);
/*
** is_check3.c
*/
int			is_empty(const char *line);
/*
** print.c
*/
char		*symboltostr(t_symbol symbol);
void		print_list(t_shell *shell);
void		print_all_env(t_shell *shell);
int			print_errno(char *object, int ret);
int			print_error(t_shell *shell);
/*
** history.c
*/
void		rebuilt_history_file(t_shell *shell, int fd);
void		update_history(t_shell *shell, t_hist *hist);
void		fill_history(t_hist *history, int fd);
/*
** free.c
*/
int			exit_free(t_shell *shell);
void		clear_parsing(t_shell *shell);
void		free_shell(t_shell *shell);
/*
** free_utils.c
*/
void		free_heredoc(t_shell *shell);
void		free_tab(char **tab);
void		free_env(t_env **env);
void		free_token(void *token);
void		free_history(t_hist *history);
/*
** msh_cd.c
*/
void		create_update(t_shell *shell, char *name, char *current_pwd);
int			update_directory_path(t_shell *shell, char *name);
int			go_home(t_shell *shell);
int			go_dir(t_shell *shell, char *path);
int			msh_cd(t_shell *shell, char **av);
/*
** msh_echo.c
*/
int			msh_echo(t_shell *shell, char **av);
/*
** msh_env.c
*/
int			msh_env(t_shell *shell, char **av);
/*
** msh_exit.c
*/
int			ft_strisdigit(const char *str);
int			msh_exit(t_shell *shell, char **av);
/*
** msh_export.c
*/
int			check_arg_format(char *arg);
void		update_value(t_shell *shell, char *arg, char *name, char **av);
void		create_value(t_shell *shell, char *arg, char *name, char **av);
void		proceed_exportation(t_shell *shell, char **av);
int			msh_export(t_shell *shell, char **av);
/*
** msh_unset.c
*/
void		remove_node(t_shell *shell, char *arg);
int			proceed_unsetting(t_shell *shell, char **av);
int			msh_unset(t_shell *shell, char **av);
/*
** msh_pwd.c
*/
int			msh_pwd(t_shell *shell);
/*
** GLOBAL VARIABLE FOR SIGNALS
*/
t_sig		g_sig;
#endif
