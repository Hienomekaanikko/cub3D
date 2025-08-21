/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:35:28 by clu               #+#    #+#             */
/*   Updated: 2025/08/18 14:46:58 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Getting the minimap view centred on the player
** Loops through all visible tiles and draw each cell
** Draw the player's position
** Draw the player's direction
*/
void	render_minimap(t_game *g)
{
	t_minimap_view	view;
	int				tx;
	int				ty;

	get_minimap_view(g, &view);
	ty = 0;
	while (ty < MINIMAP_VIEW_H)
	{
		tx = 0;
		while (tx < MINIMAP_VIEW_W)
		{
			view.x = view.start_x + tx;
			view.y = view.start_y + ty;
			draw_map_cell_view(g, &view);
			tx++;
		}
		ty++;
	}
	draw_mini_p(g, (int)view.px, (int)view.py);
	draw_mini_dir(g, &view, (int)view.px, (int)view.py);
}

/*
** Calculating parts of the map to show and centering the view on player
** Avoid out of bounds and convert player's position to minimap pixel coord
*/
void	get_minimap_view(const t_game *g, t_minimap_view *view)
{
	double	half_w;
	double	half_h;
	double	origin_x;
	double	origin_y;

	half_w = MINIMAP_VIEW_W / 2.0;
	half_h = MINIMAP_VIEW_H / 2.0;
	origin_x = g->player.x - half_w;
	origin_y = g->player.y - half_h;
	if (origin_x < 0)
		origin_x = 0;
	if (origin_y < 0)
		origin_y = 0;
	if (g->map_data.width <= MINIMAP_VIEW_W)
		origin_x = 0;
	else if (origin_x + MINIMAP_VIEW_W > g->map_data.width)
		origin_x = g->map_data.width - MINIMAP_VIEW_W;
	if (g->map_data.height <= MINIMAP_VIEW_H)
		origin_y = 0;
	else if (origin_y + MINIMAP_VIEW_H > g->map_data.height)
		origin_y = g->map_data.height - MINIMAP_VIEW_H;
	view->start_x = (int)floor(origin_x);
	view->start_y = (int)floor(origin_y);
	view->px = (g->player.x - origin_x) * MINIMAP_TILE;
	view->py = (g->player.y - origin_y) * MINIMAP_TILE;
}

/*
** Drawing single cell on minimap per tile
** Floor/player position drew in gray
** Everything else transparent
*/
void	draw_map_cell_view(t_game *g, t_minimap_view *view)
{
	char	cell;
	int		screen_x;
	int		screen_y;

	screen_x = (view->x - view->start_x) * MINIMAP_TILE;
	screen_y = (view->y - view->start_y) * MINIMAP_TILE;
	if (view->y < 0 || view->y >= g->map_data.height || view->x < 0
		|| view->x >= (int)ft_strlen(g->map_data.grid[view->y]))
	{
		draw_minimap_tile(g->minimap, screen_x, screen_y, 0x00000000);
		return ;
	}
	cell = g->map_data.grid[view->y][view->x];
	if (cell == '1')
		draw_minimap_tile(g->minimap, screen_x, screen_y, 0xFFFFFFFF);
	else if (cell == '0' || cell == 'N' || cell == 'S' || cell == 'E'
		|| cell == 'W')
		draw_minimap_tile(g->minimap, screen_x, screen_y, 0x404040FF);
	else
		draw_minimap_tile(g->minimap, screen_x, screen_y, 0x00000000);
}

/*
** Drawing a line from the player dot to indicate the view direction
*/
void	draw_mini_dir(t_game *g, t_minimap_view *view, int sx, int sy)
{
	int	i;

	init_direction_line(view, sx, sy, &g->player);
	if (view->line_steps == 0)
		return ;
	i = 0;
	while (i <= view->line_steps)
	{
		if ((int)view->line_current_x >= 0
			&& (int)view->line_current_x < MINIMAP_SIZE
			&& (int)view->line_current_y >= 0
			&& (int)view->line_current_y < MINIMAP_SIZE)
			mlx_put_pixel(g->minimap, (int)view->line_current_x,
				(int)view->line_current_y, 0x87CEEBFF);
		view->line_current_x += view->line_inc_x;
		view->line_current_y += view->line_inc_y;
		i++;
	}
}

/*
** Set up the start/end points and increments for the direction line
** Length of the line is fixed
*/
void	init_direction_line(t_minimap_view *view, int sx, int sy,
			t_player *player)
{
	view->line_start_x = sx;
	view->line_start_y = sy;
	view->line_end_x = sx + player->dir_x * MINIMAP_LINE;
	view->line_end_y = sy + player->dir_y * MINIMAP_LINE;
	view->line_steps = (int)fmax(fabs(view->line_end_x - view->line_start_x),
			fabs(view->line_end_y - view->line_start_y));
	if (view->line_steps == 0)
		return ;
	view->line_inc_x = (view->line_end_x - view->line_start_x)
		/ (double)view->line_steps;
	view->line_inc_y = (view->line_end_y - view->line_start_y)
		/ (double)view->line_steps;
	view->line_current_x = view->line_start_x;
	view->line_current_y = view->line_start_y;
}
