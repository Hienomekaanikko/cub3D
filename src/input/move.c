/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:19:44 by clu               #+#    #+#             */
/*   Updated: 2025/08/13 14:38:42 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Processing player movements WASD
*/
void	move_p(t_game *g)
{
	if (mlx_is_key_down(g->mlx, MLX_KEY_W))
		apply_movement(g, g->player.dir_x, g->player.dir_y);
	if (mlx_is_key_down(g->mlx, MLX_KEY_S))
		apply_movement(g, -g->player.dir_x, -g->player.dir_y);
	if (mlx_is_key_down(g->mlx, MLX_KEY_A))
		apply_movement(g, g->player.dir_y, -g->player.dir_x);
	if (mlx_is_key_down(g->mlx, MLX_KEY_D))
		apply_movement(g, -g->player.dir_y, g->player.dir_x);
}

/*
** Applying movements with collision detection
** and frame-rate independent speed
*/
void	apply_movement(t_game *g, double dir_x, double dir_y)
{
	double	new_x;
	double	new_y;
	double	move_speed;

	move_speed = MOVE_UNIT * g->mlx->delta_time;
	new_x = g->player.x + dir_x * move_speed;
	new_y = g->player.y + dir_y * move_speed;
	if (is_valid_pos(g, new_x, g->player.y))
		g->player.x = new_x;
	if (is_valid_pos(g, g->player.x, new_y))
		g->player.y = new_y;
}

/*
** Handling arrow key rotation of the camera view
** Rotation angle in radians accounted for delta
*/
void	rotate_p(t_game *g)
{
	double	odx;
	double	opx;
	double	rot;

	rot = ROT_RADIAN * g->mlx->delta_time;
	if (mlx_is_key_down(g->mlx, MLX_KEY_LEFT))
	{
		odx = g->player.dir_x;
		opx = g->player.plane_x;
		g->player.dir_x = odx * cos(-rot) - g->player.dir_y * sin(-rot);
		g->player.dir_y = odx * sin(-rot) + g->player.dir_y * cos(-rot);
		g->player.plane_x = opx * cos(-rot) - g->player.plane_y * sin(-rot);
		g->player.plane_y = opx * sin(-rot) + g->player.plane_y * cos(-rot);
	}
	if (mlx_is_key_down(g->mlx, MLX_KEY_RIGHT))
	{
		odx = g->player.dir_x;
		opx = g->player.plane_x;
		g->player.dir_x = odx * cos(rot) - g->player.dir_y * sin(rot);
		g->player.dir_y = odx * sin(rot) + g->player.dir_y * cos(rot);
		g->player.plane_x = opx * cos(rot) - g->player.plane_y * sin(rot);
		g->player.plane_y = opx * sin(rot) + g->player.plane_y * cos(rot);
	}
}

/*
** Handling the mouse rotation
** Mouse position is stored when TAB toggle activates
** rot threshold set to very small to control very minor rotation
*/
void	handle_mouse(t_game *g)
{
	double	delta_x;
	double	rot;
	double	odx;
	double	opx;

	if (!g->mouse_disabled)
		return ;
	init_mouse_position(g);
	if (g->first_mouse == false)
	{
		delta_x = g->mouse_x - g->last_mouse_x;
		rot = delta_x * 0.001;
		if (fabs(rot) > 0.0001)
		{
			odx = g->player.dir_x;
			opx = g->player.plane_x;
			g->player.dir_x = odx * cos(rot) - g->player.dir_y * sin(rot);
			g->player.dir_y = odx * sin(rot) + g->player.dir_y * cos(rot);
			g->player.plane_x = opx * cos(rot) - g->player.plane_y * sin(rot);
			g->player.plane_y = opx * sin(rot) + g->player.plane_y * cos(rot);
		}
		g->last_mouse_x = g->mouse_x;
		g->last_mouse_y = g->mouse_y;
	}
}

/*
** Setting and storing the mouse position information
*/
void	init_mouse_position(t_game *g)
{
	mlx_get_mouse_pos(g->mlx, &g->mouse_x, &g->mouse_y);
	if (g->first_mouse)
	{
		g->last_mouse_x = g->mouse_x;
		g->last_mouse_y = g->mouse_y;
		g->first_mouse = false;
	}
}
