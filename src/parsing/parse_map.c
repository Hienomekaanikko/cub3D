/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:49:18 by msuokas           #+#    #+#             */
/*   Updated: 2025/08/18 11:59:03 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	set_line_data(t_game *game, char *line, int *status, int i)
{
	if (line[i] != '1' && line[i] != '0' && line[i] != 'W'
		&& line[i] != 'S' && line[i] != 'E' && line[i] != ' '
		&& line[i] != 'N' && line[i] != '\n')
	{
		*status = -13;
		return (0);
	}
	if (line[i] == 'N' || line[i] == 'S'
		|| line[i] == 'E' || line[i] == 'W')
	{
		if (game->map_data.start_dir == '0')
		{
			game->map_data.start_dir = line[i];
			game->map_data.start_x = i;
			game->map_data.start_y = game->map_data.height;
		}
	}
	return (1);
}

//updates width, height, player position if encountered in a line
//checks invalid characters
void	handle_line_data(t_game *game, char *line, int *status)
{
	int	i;
	int	current_width;

	i = 0;
	if (*status < 1)
		return ;
	current_width = ft_strlen(line);
	if (current_width > game->map_data.width)
		game->map_data.width = current_width;
	while (line[i])
	{
		if (!set_line_data(game, line, status, i))
			return ;
		i++;
	}
}

//makes the map_data.grid
int	set_grid(t_game *game, char *temp_map, int status)
{
	if (status < 1)
		return (status);
	game->map_data.grid = ft_split(temp_map, '\n');
	if (!game->map_data.grid)
		return (-1);
	return (1);
}

//flood fill function to ensure that the map does not have holes in it
int	check_map_is_closed(char **grid_cpy, t_map_data map_data, int x, int y)
{
	char	c;

	if (x >= (int)ft_strlen(grid_cpy[y]))
		return (0);
	if (x < 0 || y < 0 || x >= map_data.width || y >= map_data.height)
		return (0);
	c = grid_cpy[y][x];
	if (c == ' ' || c == '\0' || c == '\n')
		return (0);
	if (c == '1' || c == '.')
		return (1);
	grid_cpy[y][x] = '.';
	if (!check_map_is_closed(grid_cpy, map_data, x + 1, y))
		return (0);
	if (!check_map_is_closed(grid_cpy, map_data, x - 1, y))
		return (0);
	if (!check_map_is_closed(grid_cpy, map_data, x, y + 1))
		return (0);
	if (y == 0)
		return (0);
	if (!check_map_is_closed(grid_cpy, map_data, x, y - 1))
		return (0);
	return (1);
}

//makes sure the map is not empty and has needed player component and no extra
int	check_map_validity(char *temp_map, int status)
{
	int	i;
	int	player_count;

	if (status < 1)
		return (status);
	i = 0;
	player_count = 0;
	if (temp_map[0] == '\0')
		return (-6);
	while (temp_map[i])
	{
		if (temp_map[i] == 'N' || temp_map[i] == 'S'
			|| temp_map[i] == 'E' || temp_map[i] == 'W')
			player_count++;
		i++;
	}
	if (player_count != 1)
		return (-5);
	return (1);
}
