/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:36:06 by clu               #+#    #+#             */
/*   Updated: 2025/08/18 11:44:08 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Loading the textures for 4 walls in the correct order
*/
int	load_textures(t_game *g)
{
	int				i;
	const char		*tex_paths[4] = {
		g->map_data.no_tex,
		g->map_data.so_tex,
		g->map_data.ea_tex,
		g->map_data.we_tex,
	};

	i = 0;
	while (i < 4)
	{
		if (!load_a_texture(g, tex_paths[i], i))
		{
			cleanup_textures(g);
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** Loading each PNG texture file and convert to mlx42 image
*/
int	load_a_texture(t_game *g, const char *path, int index)
{
	mlx_texture_t	*tex;

	tex = mlx_load_png(path);
	if (!tex)
	{
		printer("Failed to load texture from PNG", NULL);
		return (0);
	}
	g->textures[index] = mlx_texture_to_image(g->mlx, tex);
	mlx_delete_texture(tex);
	if (!g->textures[index])
	{
		printer("Failed to load image from textures", NULL);
		return (0);
	}
	return (1);
}

/*
** Setup of the player direction, N/S
** Initializing the view direction and camera plane based on orientation
*/
void	set_p_dir_n_s(t_game *g)
{
	if (g->map_data.start_dir == 'N')
	{
		g->player.dir_x = 0.0;
		g->player.dir_y = -1.0;
		g->player.plane_x = FOV;
		g->player.plane_y = 0.0;
	}
	else if (g->map_data.start_dir == 'S')
	{
		g->player.dir_x = 0.0;
		g->player.dir_y = 1.0;
		g->player.plane_x = -FOV;
		g->player.plane_y = 0.0;
	}
}

/*
** Setup of the player direction, N/S
** Initializing the view direction and camera plane based on orientation
*/
void	set_p_dir_e_w(t_game *g)
{
	if (g->map_data.start_dir == 'E')
	{
		g->player.dir_x = 1.0;
		g->player.dir_y = 0.0;
		g->player.plane_x = 0.0;
		g->player.plane_y = FOV;
	}
	else if (g->map_data.start_dir == 'W')
	{
		g->player.dir_x = -1.0;
		g->player.dir_y = 0.0;
		g->player.plane_x = 0.0;
		g->player.plane_y = -FOV;
	}
}
