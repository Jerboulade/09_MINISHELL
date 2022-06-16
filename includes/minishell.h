/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 22:35:31 by jcarere           #+#    #+#             */
/*   Updated: 2022/06/16 02:58:30 by jcarere          ###   ########.fr       */
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

typedef enum e_symbol
{
	T_COMMAND,
	T_BUILTIN,
	T_ARG,
	T_PIPE,
	T_REDIRECT,
	T_TEST
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
typedef struct	s_parg
{
	char		*line;
	int			pos;
	int			ret;
	// int			i;
	// int			j;
	int 		error_line;
	char 		*error_file;

}				t_parg;

typedef struct	s_token
{
	int			pos;
	char		*key;
	t_symbol	symbol;
}				t_token;

typedef struct s_shell
{
	t_list	**start;
	t_list	*current;
}
			t_shell;

#endif
