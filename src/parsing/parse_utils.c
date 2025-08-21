/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:50:42 by msuokas           #+#    #+#             */
/*   Updated: 2025/08/18 10:17:32 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_stuff(t_game *game)
{
	if (game->map_data.grid)
		ft_free_split(game->map_data.grid);
	if (game->map_data.no_tex)
		free(game->map_data.no_tex);
	if (game->map_data.so_tex)
		free(game->map_data.so_tex);
	if (game->map_data.ea_tex)
		free(game->map_data.ea_tex);
	if (game->map_data.we_tex)
		free(game->map_data.we_tex);
}

void	missing_elements(t_game *game, int *err_code)
{
	if (*err_code > 0 && (!game->map_data.no_tex || !game->map_data.so_tex
			|| !game->map_data.we_tex || !game->map_data.ea_tex))
	{
		*err_code = 0;
		printer("Invalid texture setup inside .cub file!", NULL);
	}
	if (*err_code == -10)
		printer("Texture file not accessable!", NULL);
	if (*err_code == -13)
		printer("Invalid characters in the map!", NULL);
	if (*err_code == -14)
		printer("Invalid file configuration", NULL);
	if (*err_code == -15)
		printer("Texture element is a directory", NULL);
}

void	error(t_game *game, int *err_code, char *path)
{
	if (*err_code == -1)
		printer("Malloc failed during the parsing of file!", path);
	if (*err_code == -2)
		printer("Invalid map", NULL);
	if (*err_code == -3)
		printer("RGB must have 3 numeric values!", NULL);
	if (*err_code == -4)
		printer("RGB values must be in range 0–255!", NULL);
	if (*err_code == -5)
		printer("Set ONE starting position!", NULL);
	if (*err_code == -6)
		printer("Empty/invalid file contents", NULL);
	if (*err_code == -7)
		printer("Set floor color only once!", NULL);
	if (*err_code == -8)
		printer("Set ceiling color only once!", NULL);
	if (*err_code == -9)
		printer("More than one instance per texture!", NULL);
	if (*err_code < -9)
		missing_elements(game, err_code);
	if (*err_code < 1)
		file_guide();
}

//guide for how to fill the file
void	file_guide(void)
{
	ft_putstr_fd("\033[36m", 2);
	ft_putendl_fd("Correct format: ", 2);
	ft_putendl_fd("-----------------------------", 2);
	ft_putendl_fd("NO ./path_to_the_north_texture", 2);
	ft_putendl_fd("SO ./path_to_the_south_texture", 2);
	ft_putendl_fd("EA ./path_to_the_east_texture", 2);
	ft_putendl_fd("WE ./path_to_the_west_texture\n", 2);
	ft_putendl_fd("F [R],[G],[B]", 2);
	ft_putendl_fd("C [R],[G],[B]\n", 2);
	ft_putendl_fd("<Map>", 2);
	ft_putstr_fd("\033[0m", 2);
}

void	printer(char *msg, char *identifyer)
{
	ft_putendl_fd("\033[31mError\033[0m", 2);
	ft_putstr_fd("\033[36m", 2);
	ft_putstr_fd(msg, 2);
	if (identifyer)
	{
		ft_putstr_fd("\033[36m", 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(identifyer, 2);
		ft_putstr_fd("\033[0m", 2);
	}
	ft_putstr_fd("\n", 2);
}
