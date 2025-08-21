/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:36:31 by clu               #+#    #+#             */
/*   Updated: 2025/08/14 13:33:26 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Cast all rays across the screen width
void	cast_rays(t_game *g)
{
	t_ray	ray;
	int		x;

	x = 0;
	while (x < WIN_W)
	{
		init_ray(g, &ray, x);
		if (perform_dda(g, &ray))
			draw_wall_slice(g, x, &ray);
		x++;
	}
}

// Initialize ray parameters for a given screen column
void	init_ray(t_game *g, t_ray *ray, int x)
{
	setup_ray_direction(g, ray, x);
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_x = (g->player.x - ray->map_x) * ray->delta_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_x = (ray->map_x + 1.0 - g->player.x) * ray->delta_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_y = (g->player.y - ray->map_y) * ray->delta_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_y = (ray->map_y + 1.0 - g->player.y) * ray->delta_y;
	}
}

/*
** Setting camera screen based on screen size width
** Build the ray with player direction, plane, and camera screen
** Ray moving along the vertical grid
** Ray moving along the horizontal grid
*/
void	setup_ray_direction(t_game *g, t_ray *ray, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WIN_W - 1;
	ray->dir_x = g->player.dir_x + g->player.plane_x * camera_x;
	ray->dir_y = g->player.dir_y + g->player.plane_y * camera_x;
	ray->map_x = (int)g->player.x;
	ray->map_y = (int)g->player.y;
	ray->delta_x = fabs(1 / ray->dir_x);
	ray->delta_y = fabs(1 / ray->dir_y);
}

/*
** Perform DDA algo to find wall intersection
** Step through the grid based on ray direction
** If ray hits a wall, calculate the perpendicular distance
** Returns 1 when loop ends, indicating a wall hit
*/
int	perform_dda(t_game *g, t_ray *ray)
{
	int	hit;

	hit = 0;
	while (!hit)
	{
		if (ray->side_x < ray->side_y)
		{
			ray->side_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		hit = check_ray_hit(g, ray);
	}
	return (1);
}

/*
** Check if ray hit wall or went out of bounds
** If hit a wall, calculate the perpendicular distance
** Returns 1 if hit a wall, 0 if no hit
*/
int	check_ray_hit(t_game *g, t_ray *ray)
{
	if (g->map_data.grid[ray->map_y][ray->map_x] == '1')
	{
		if (ray->side == 0)
			ray->perp_wall = (ray->map_x - g->player.x + (1 - ray->step_x) / 2)
				/ ray->dir_x;
		else
			ray->perp_wall = (ray->map_y - g->player.y + (1 - ray->step_y) / 2)
				/ ray->dir_y;
		return (1);
	}
	return (0);
}
