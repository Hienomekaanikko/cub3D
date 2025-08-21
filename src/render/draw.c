/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:36:26 by clu               #+#    #+#             */
/*   Updated: 2025/08/12 17:21:48 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Calculate the wall height and position
** Determine the wall texture with corresponding direction
** Draw the ceiling and floor above/below the wall
** Set and draw the wall slices
*/
void	draw_wall_slice(t_game *g, int x, t_ray *ray)
{
	mlx_image_t	*texture;

	calc_wall_bound(ray, &g->bounds.line_height, &g->bounds.draw_start,
		&g->bounds.draw_end);
	g->bounds.wall_dir = get_wall_textures(ray);
	texture = g->textures[g->bounds.wall_dir];
	draw_background(g, x, g->bounds.draw_start, g->bounds.draw_end);
	setup_wall_texture(g, ray, texture);
	draw_textured_wall(g, x, ray);
}

/*
** Draw ceiling from the top of the window to the top of the wall
** Draw floor from end of wall to the bottom
*/
void	draw_background(t_game *g, int x, int draw_start, int draw_end)
{
	int	y;

	y = 0;
	while (y < draw_start)
	{
		mlx_put_pixel(g->img, x, y, g->map_data.ceiling_color);
		y++;
	}
	y = draw_end + 1;
	while (y < WIN_H)
	{
		mlx_put_pixel(g->img, x, y, g->map_data.floor_color);
		y++;
	}
}

/*
** Compute the position where the ray hits the wall: wall_x
** Get the fraction of the position within a wall tile
** Calculate how much to move vertically in texture per pixel drawn
** Set vertical position for the wall slice texture
*/
void	setup_wall_texture(t_game *g, t_ray *ray, mlx_image_t *texture)
{
	if (ray->side == 0)
		g->tex.wall_x = g->player.y + ray->perp_wall * ray->dir_y;
	else
		g->tex.wall_x = g->player.x + ray->perp_wall * ray->dir_x;
	g->tex.wall_x -= floor(g->tex.wall_x);
	g->tex.tex_x = (int)(g->tex.wall_x * texture->width);
	if (g->tex.tex_x < 0)
		g->tex.tex_x = 0;
	if (g->tex.tex_x >= (int)texture->width)
		g->tex.tex_x = texture->width - 1;
	g->tex.step = 1.0 * texture->height / g->bounds.line_height;
	g->tex.tex_pos = (g->bounds.draw_start - WIN_H / 2
			+ g->bounds.line_height / 2) * g->tex.step;
}

/*
** Draw textured wall section
*/
void	draw_textured_wall(t_game *g, int x, t_ray *ray)
{
	uint32_t	color;
	int			tex_y;
	int			y;
	mlx_image_t	*texture;

	texture = g->textures[g->bounds.wall_dir];
	y = g->bounds.draw_start;
	while (y <= g->bounds.draw_end)
	{
		tex_y = (int)g->tex.tex_pos;
		tex_y = tex_y & (texture->height - 1);
		g->tex.tex_pos += g->tex.step;
		color = get_texture_pixel(texture, g->tex.tex_x, tex_y);
		color = apply_distance_fog(color, ray->perp_wall);
		mlx_put_pixel(g->img, x, y, color);
		y++;
	}
}
