/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:42:29 by msuokas           #+#    #+#             */
/*   Updated: 2025/08/20 11:45:01 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//extracts the path that comes after NO, SO, EA or WE
static char	*get_texture_path(char *line, int *status)
{
	int		i;
	char	*path;

	i = 2;
	if (line[i] != ' ')
		return (NULL);
	if (has_illegal_spaces(line))
		return (NULL);
	while (line[i] == ' ')
		i++;
	path = ft_strtrim(&line[i], "\n");
	if (!path)
	{
		*status = -1;
		return (NULL);
	}
	return (path);
}

//sets NO, SO, WE, EA, F, C to it's place
static void	place_texture(char **txt, char *line, int *status)
{
	if (*txt)
	{
		*status = -9;
		return ;
	}
	else
	{
		*txt = get_texture_path(line, status);
		if (!(*txt) && *status == 1)
		{
			*status = -14;
			return ;
		}
		if (access(*txt, R_OK | W_OK) == 0)
		{
			if (is_directory(*txt))
				*status = -15;
			return ;
		}
		else
			*status = -10;
	}
}

// if a line in the .cub file has any of the key words
// (NO, SO, WE, EA, F, C), it saves the following content into map_data
static int	set_elem_paths(t_game *game, char *line, int *status)
{
	int	i;

	i = 0;
	if (*status < 1)
		return (0);
	while (line[i] && line[i] == ' ')
		i++;
	if (line[i] && line[i + 1] && ft_strncmp(&line[i], "NO", 2) == 0)
		place_texture(&game->map_data.no_tex, &line[i], status);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "SO", 2) == 0)
		place_texture(&game->map_data.so_tex, &line[i], status);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "WE", 2) == 0)
		place_texture(&game->map_data.we_tex, &line[i], status);
	else if (line[i] && line[i + 1] && ft_strncmp(&line[i], "EA", 2) == 0)
		place_texture(&game->map_data.ea_tex, &line[i], status);
	else if (line[i] && (line[i] == 'F' || line[i] == 'C'))
		place_color(game, line, i, status);
	else if (line[i] == '\0' || line[i] == '\n')
		return (1);
	else if (ready_for_map(game, status, line, i))
		return (0);
	else
		*status = -14;
	return (1);
}

//loops through the .cub file with gnl
static int	set_file_contents(t_game *g, int fd, char **tmp_map, char **joined)
{
	char	*line;
	int		status;

	status = 1;
	line = get_next_line(fd);
	while (line)
	{
		if (!*joined && set_elem_paths(g, line, &status))
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		*joined = ft_strjoin(*tmp_map, line);
		if (!*joined)
			status = -1;
		free(*tmp_map);
		empty_line_midmap(line, &status, *joined);
		handle_line_data(g, line, &status);
		*tmp_map = *joined;
		free(line);
		line = get_next_line(fd);
		g->map_data.height++;
	}
	return (status);
}

// goes through the .cub file and directs the
// contents to the right places in map_data
int	parse_cub_file(t_game *game, int fd, char **temp_map)
{
	char	**grid_cpy;
	char	*joined;
	int		status;
	int		x;
	int		y;

	joined = NULL;
	status = set_file_contents(game, fd, temp_map, &joined);
	status = check_map_validity(*temp_map, status);
	status = set_grid(game, *temp_map, status);
	if (status < 1)
		return (status);
	grid_cpy = ft_split(*temp_map, '\n');
	if (!grid_cpy)
		return (-1);
	x = game->map_data.start_x;
	y = game->map_data.start_y;
	if (!check_map_is_closed(grid_cpy, game->map_data, x, y))
		status = -2;
	if (elements_outside(grid_cpy))
		status = -2;
	ft_free_split(grid_cpy);
	grid_cpy = NULL;
	return (status);
}
