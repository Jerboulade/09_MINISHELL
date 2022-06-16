/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 16:35:50 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/lib_includes/libft.h"
# include <sys/param.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define CYAN "\033[1;36m"
# define GREEN "\033[1;32m"
# define MAG "\033[1;35m"
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
	T_COMMAND,
	T_BUILTIN,
	T_ARG,
	T_PIPE,
	T_REDIRECT,
	T_TEST
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
	t_list		**start;
	t_list		*current;
}				t_shell;
/*
** minishell.c
*/
void	display_prompt(void);
t_shell	*init_shell(void);
void	minishell(void);
/*
** parsing.c
*/
char	*red_key(t_parg *parg, int *i);
char	*arg_key(t_parg *parg, int start, int end);
int		get_token(t_shell *shell, t_parg *parg, int *i, int j);
int		stop_parsing(t_shell *shell, t_parg *parg, int i);
int		parsing(t_shell *shell, t_parg *parg, int i);
/*
** tokenizer.c
*/
int		tokenizer(t_shell *shell, t_parg *parg, char *key);
/*
** parsing_utils.c
*/
size_t	ft_skipcharlen(const char *str, char c);
int		is_end(char *line, int i);
int		is_start(char *line, int i);
int		set_trigger(t_parg *parg, char *file, int errline, int ret);
void	set_quote(char *quote, char *line);
/*
** print.c
*/
void	print_list(t_shell *shell);
int		print_parserror(t_parg *parg, int i);
/*
** free.c
*/
void	free_parg(t_parg *parg);
void	free_token(void *token);
void	free_shell(t_shell *shell);
#endif
