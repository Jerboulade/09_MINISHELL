/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 21:01:48 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/13 01:42:37 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		append_backslash(char **path_tab)
{
	int			i;
	char		*tmp;

	i = 0;
	while (path_tab[i])
		i++;
	while (i--)
	{
		if (path_tab[i][ft_strlen(path_tab[i]) - 1] != '/')
		{
			tmp = ft_strjoin(path_tab[i], "/");
			if (!tmp)
				return (0);
			free(path_tab[i]);
			path_tab[i] = tmp;
		}
	}
	return (1);
}

t_list	*find_nextflag(t_shell *shell)
{
	t_list	*tmp;
	int		moveflag;

	tmp = shell->current;
	while (pop_symbol(tmp) != T_PIPE && pop_symbol(tmp) != T_NEWLINE)
		tmp = tmp->next;
	moveflag = pop_index(tmp);
	while (tmp->next && pop_index(tmp->next) < moveflag)
		tmp = tmp->next;
	return (tmp);
}

void	merge_and_move(t_shell *shell, t_list *tmp_prev, int moveflag)
{
	t_list	*tmp_next;
	t_list	*tmp_curr;

	if (pop_key(shell->current) == NULL)
	{
		tmp_next = shell->current->next;
		pop_token(shell->current)->key = pop_key(tmp_next);
		pop_token(tmp_next)->key = NULL;
		shell->current->next = shell->current->next->next;
		ft_lstdelone(tmp_next, &free_token);
	}
	if (!moveflag || (moveflag && pop_index(shell->current) > moveflag))
	{
		tmp_next = find_nextflag(shell);
		tmp_curr = shell->current;
		tmp_prev->next = tmp_curr->next;
		shell->current = tmp_prev;
		tmp_prev = tmp_next;
		tmp_next = tmp_prev->next;
		tmp_prev->next = tmp_curr;
		tmp_curr->next = tmp_next;
	}
}

// int	file_check(t_shell *shell)
// {
// 	// int fd;
// 	// t_symbol symbol;
// 	//
// 	shell->current = shell->start;
// 	// while (shell->current)
// 	// {
// 	// 	symbol = pop_symbol(shell->current);
// 	// 	if (symbol == T_REDIRIN)
// 	// 	{
// 	// 		fd = open(pop_key(shell->current), O_RDONLY, 0666);
// 	// 		if (fd == -1) //&& errno)
// 	// 			return (errno);
// 	// 		close(fd);
// 	// 	}
// 	// 	else if (symbol == T_REDIROUT)
// 	// 	{
// 	// 		fd = open(pop_key(shell->current), O_RDONLY, 0666);
// 	// 		if (fd == -1) //&& errno)
// 	// 			return (errno);
// 	// 		close(fd);
// 	// 	}
// 	// 	shell->current = shell->current->next;
// 	// }
// 	return (0);
// }
char	**build_pathtab(t_shell *shell, char *key)
{
	int		i;
	char	*tmp;
	char	**path_tab;
	char	*paths;

	paths = get_env(shell, "PATH");
	if (!paths)
		return (NULL);
	path_tab = ft_split(paths, ':');
	if (!path_tab)
		exit_free(shell);
	if (!append_backslash(path_tab))
		exit_free(shell);
	i = -1;
	while (path_tab[++i])
	{
		tmp = ft_strjoin(path_tab[i], key);
		if (!tmp)
			exit_free(shell);
		free(path_tab[i]);
		path_tab[i] = tmp;
	}
	// ft_printf("%s############# ENV_PATH ################\n", MAG);
	// i = -1;
	// while (path_tab[++i])
	// 	ft_printf("[%02d] %s\n", i, path_tab[i]);
	// ft_printf("%s\n", RESET);
	return (path_tab);
}

int	check_bin_path(char *path)
{
	int			stat_ret;
	struct stat	info;

	stat_ret = stat(path, &info);
	if (stat_ret != 0) //not found
		return (127);
	if (stat_ret == 0 && (info.st_mode & S_IFMT) == S_IFDIR)
		return (126);
	return (0);
}

void	update_bin_path(t_shell *shell, t_token *token, char *path)
{
	free(token->key);
	token->key = ft_strdup(path);
	if (!token->key)
		exit_free(shell);
}

int	search_bin_path(t_shell *shell, t_token *token)
{
	int			i;
	int			ret;
	char		**path_tab;

	if (!token->key[0])
	 	return (127);
	ret = check_bin_path(token->key);
	if (!ret || ft_strchr(token->key, '/'))
		return (ret);
	path_tab = build_pathtab(shell, token->key);
	if (!path_tab)
		return (127);
	i = -1;
	while (path_tab[++i])
	{
		ret = check_bin_path(path_tab[i]);
		if (!ret || ret == 126)
		{
			if (!ret)
				update_bin_path(shell, token, path_tab[i]);
			break;
		}
	}
	free_tab(path_tab);
	return (ret);
}

int	is_relbin(char *key)
{
	struct stat	info;

	if (stat(key, &info) == 0 && (info.st_mode & S_IFMT) != S_IFDIR)
	{
		// ft_printf("%s is (S_KEY = %d)\n", key, info.st_mode & S_IFMT);
		// ft_printf("S_IFDIR = %d\nS_IFREG = %d\nS_IFCHR = %d\nS_IFBLK = %d\nS_IFLNK = %d\nS_IFIFO = %d\nS_IFSOCK = %d\n", S_IFDIR, S_IFREG, S_IFCHR, S_IFBLK, S_IFLNK, S_IFIFO, S_IFSOCK);
		return (1);
	}
	return (0);
}

int	search_command(t_shell *shell)
{
	int	ret;

	if (is_builtin(pop_key(shell->current)))
		pop_token(shell->current)->symbol = T_BUILTIN;
	else
	{
		ret = search_bin_path(shell, pop_token(shell->current));
		if (ret)
			return (ret);
		pop_token(shell->current)->symbol = T_BIN;
	}
	return (0);
}

int	lexer(t_shell *shell)
{
	// ft_printf("%s#######################################\n", CYAN);
	// ft_printf("                  LEXER                  \n");
	// ft_printf("#######################################%s\n", RESET);
	t_list	*tmp_prev;
	int		moveflag;
	int		ret;

	moveflag = 0;
	if (is_redir(pop_symbol(shell->current)))
		return (5);
	token_add_newline(shell);
	shell->current = shell->start;
	while (pop_symbol(shell->current) != T_NEWLINE)
	{
		if (pop_symbol(shell->current) == T_WORD && !pop_pos(shell->current))
		{
			ret = search_command(shell);
			if (ret)
				return (ret);
		}
		else if (is_redir(pop_symbol(shell->current)))
			merge_and_move(shell, tmp_prev, moveflag);
		else if (is_flag(pop_symbol(shell->current)))
			moveflag = pop_index(shell->current);
		tmp_prev = shell->current;
		shell->current = shell->current->next;
	}
	return (0);
}
