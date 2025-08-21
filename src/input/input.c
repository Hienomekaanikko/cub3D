/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:36:12 by clu               #+#    #+#             */
/*   Updated: 2025/08/13 14:37:51 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Checking of the player input at every frame
** Verifying game state and process player movements WASD
** Process arrow key and mouse movements for view rotation
** Perform 3D raycasting and render minimap overlay
*/
void	handle_input(void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (!g || !g->img)
		return ;
	move_p(g);
	rotate_p(g);
	handle_mouse(g);
	cast_rays(g);
	render_minimap(g);
}

/*
** Processing keypresses: ESC to exit and TAB to toggle mouse cursor
** Mouse mode toggle
*/
void	handle_keypress(mlx_key_data_t keydata, void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(g->mlx);
	if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
	{
		if (g->mouse_disabled)
		{
			mlx_set_cursor_mode(g->mlx, MLX_MOUSE_NORMAL);
			g->mouse_disabled = false;
			g->first_mouse = true;
		}
		else
		{
			mlx_set_cursor_mode(g->mlx, MLX_MOUSE_DISABLED);
			g->mouse_disabled = true;
			g->first_mouse = true;
		}
	}
}

/*
** Player position validation for wall collision check
*/
int	is_valid_pos(t_game *g, double x, double y)
{
	double	radius;
	int		min_x;
	int		max_x;
	int		min_y;
	int		max_y;

	radius = 0.1;
	min_x = (int)(x - radius);
	max_x = (int)(x + radius);
	min_y = (int)(y - radius);
	max_y = (int)(y + radius);
	if (min_x < 0 || max_x >= g->map_data.width || min_y < 0
		|| max_y >= g->map_data.height)
		return (0);
	if (g->map_data.grid[min_y] != NULL && g->map_data.grid[max_y] != NULL
		&& g->map_data.grid[min_y][min_x] != '1'
		&& g->map_data.grid[min_y][max_x] != '1'
		&& g->map_data.grid[max_y][min_x] != '1'
		&& g->map_data.grid[max_y][max_x] != '1')
		return (1);
	return (0);
}
