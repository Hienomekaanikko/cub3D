/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_base_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:00:23 by msuokas           #+#    #+#             */
/*   Updated: 2025/08/18 11:58:47 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	has_illegal_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\t' || line[i] == '\r'
			|| line[i] == '\v' || line[i] == '\f')
			return (1);
		i++;
	}
	return (0);
}

int	is_directory(char *cmd)
{
	int	fd;

	if (!cmd)
		return (0);
	fd = open(cmd, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		return (1);
	}
	return (0);
}

int	ready_for_map(t_game *game, int *status, char *line, int i)
{
	if (ft_isdigit(line[i]))
	{
		if (game->map_data.ceiling_color_set
			&& game->map_data.floor_color_set
			&& game->map_data.no_tex
			&& game->map_data.so_tex
			&& game->map_data.ea_tex
			&& game->map_data.we_tex)
			return (1);
		else
			*status = -14;
	}
	return (0);
}

static int	validate_cub_file(char *path)
{
	int	fd;
	int	len;

	len = ft_strlen(path);
	ft_putstr_fd("\033[31m", 2);
	if (len < 4 || ft_strncmp(path + len - 4, ".cub", 4) != 0)
	{
		printer("Invalid file", path);
		return (-1);
	}
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (-1);
	ft_putstr_fd("\033[0m", 2);
	return (fd);
}

// validates the map and if 'all is good',
// it sets all the elements to the t_map_data struct
int	setup_base(t_game *game, char *path)
{
	int		fd;
	int		status;
	char	*temp_map;

	status = 1;
	fd = validate_cub_file(path);
	if (fd == -1)
		return (0);
	temp_map = ft_strdup("");
	if (!temp_map)
	{
		status = -1;
		error(game, &status, path);
		return (0);
	}
	status = parse_cub_file(game, fd, &temp_map);
	error(game, &status, path);
	if (temp_map)
		free(temp_map);
	close(fd);
	if (status < 1)
		return (0);
	return (1);
}
