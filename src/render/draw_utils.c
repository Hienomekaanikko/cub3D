/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:46:36 by clu               #+#    #+#             */
/*   Updated: 2025/08/12 17:21:28 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Calculating the height and vertical position of a wall slice
*/
void	calc_wall_bound(t_ray *ray, int *line_height, int *draw_start,
					int *draw_end)
{
	*line_height = (int)(WIN_H / ray->perp_wall);
	*draw_start = -(*line_height) / 2 + WIN_H / 2;
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = (*line_height) / 2 + WIN_H / 2;
	if (*draw_end >= WIN_H)
		*draw_end = WIN_H - 1;
}

/*
** Determine wall color based on ray direction and side
*/
int	get_wall_textures(t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->dir_x > 0)
			return (EAST);
		else
			return (WEST);
	}
	else
	{
		if (ray->dir_y > 0)
			return (SOUTH);
		else
			return (NORTH);
	}
}

/*
** Applying the distance fog
** Multiply the colour by the intensity calculated based on distance
*/
uint32_t	apply_distance_fog(uint32_t color, double distance)
{
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;
	uint32_t	a;

	r = (color >> 24) & 0xFF;
	g = (color >> 16) & 0xFF;
	b = (color >> 8) & 0xFF;
	a = color & 0xFF;
	r = (uint32_t)(r * calc_fog_intensity(distance));
	g = (uint32_t)(g * calc_fog_intensity(distance));
	b = (uint32_t)(b * calc_fog_intensity(distance));
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

/*
** Calculate fog intensity based on distance
** Smaller intensity level when distance is greater
*/
double	calc_fog_intensity(double distance)
{
	double	intensity;

	intensity = 1.0 / (1.0 + distance * 0.15);
	if (intensity > 1.0)
		intensity = 1.0;
	if (intensity < 0.2)
		intensity = 0.2;
	return (intensity);
}
