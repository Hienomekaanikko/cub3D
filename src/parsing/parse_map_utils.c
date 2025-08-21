/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:49:56 by msuokas           #+#    #+#             */
/*   Updated: 2025/08/18 11:41:14 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	empty_line_midmap(char *line, int *status, char *joined)
{
	char	*trimmed;

	if (*status < 0)
		return ;
	if (joined)
	{
		trimmed = ft_strtrim(line, "\n");
		if (!trimmed)
			*status = -1;
		if ((*status == 1 && !trimmed) || *trimmed == '\0')
			*status = -2;
		free(trimmed);
	}
}

static char	is_elem(char elem)
{
	return (elem == '1' || elem == '0' || elem == '.'
		|| elem == 'S' || elem == 'W' || elem == 'E'
		|| elem == 'N');
}

void	mark_connected_parts(char **grid_cpy, int y, int x)
{
	size_t	len_current;

	if (y < 0 || x < 0 || !grid_cpy[y])
		return ;
	len_current = ft_strlen(grid_cpy[y]);
	if (x >= (int)len_current || !is_elem(grid_cpy[y][x]))
		return ;
	grid_cpy[y][x] = 'v';
	if (x + 1 < (int)len_current && is_elem(grid_cpy[y][x + 1]))
		mark_connected_parts(grid_cpy, y, x + 1);
	if (x > 0 && is_elem(grid_cpy[y][x - 1]))
		mark_connected_parts(grid_cpy, y, x - 1);
	if (grid_cpy[y + 1]
		&& x < (int)ft_strlen(grid_cpy[y + 1])
		&& is_elem(grid_cpy[y + 1][x]))
		mark_connected_parts(grid_cpy, y + 1, x);
	if (y > 0 && grid_cpy[y - 1]
		&& x < (int)ft_strlen(grid_cpy[y - 1])
		&& is_elem(grid_cpy[y - 1][x]))
		mark_connected_parts(grid_cpy, y - 1, x);
}

int	separate_islands(char **grid_cpy)
{
	int	y;
	int	x;

	y = 0;
	x = 0;
	while (grid_cpy[y])
	{
		x = 0;
		while (grid_cpy[y][x])
		{
			if (is_elem(grid_cpy[y][x]))
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	elements_outside(char **grid_cpy)
{
	int	y;
	int	x;
	int	status;

	y = 0;
	status = 0;
	while (grid_cpy[y] && !status)
	{
		x = 0;
		while (grid_cpy[y][x])
		{
			if (is_elem(grid_cpy[y][x]))
			{
				mark_connected_parts(grid_cpy, y, x);
				status = 1;
				break ;
			}
			x++;
		}
		y++;
	}
	if (separate_islands(grid_cpy))
		return (1);
	return (0);
}
