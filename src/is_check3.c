/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_check3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 23:39:17 by jcarere           #+#    #+#             */
/*   Updated: 2022/07/15 01:16:25 by jcarere          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty(const char *line)
{
	if (!line || !*line)
		return (1);
	while (*line)
	{
		if (!is_whitespace(*line))
			return (0);
		++line;
	}
	return (1);
}
