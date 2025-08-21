/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:09:18 by clu               #+#    #+#             */
/*   Updated: 2025/08/20 11:32:19 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Drawing the minimap tile on the screen
*/
void	draw_minimap_tile(mlx_image_t *img, int px, int py, uint32_t color)
{
	int	i;
	int	j;

	i = 0;
	while (i < MINIMAP_TILE)
	{
		j = 0;
		while (j < MINIMAP_TILE)
		{
			mlx_put_pixel(img, px + i, py + j, color);
			j++;
		}
		i++;
	}
}

/*
** Draw player dot on the minimap
*/
void	draw_mini_p(t_game *g, int cx, int cy)
{
	int	r;
	int	dy;
	int	dx;

	r = 4;
	dy = -r;
	while (dy <= r)
	{
		dx = -r;
		while (dx <= r)
		{
			if (dx * dx + dy * dy <= r * r
				&& cx + dx >= 0 && cx + dx < MINIMAP_SIZE
				&& cy + dy >= 0 && cy + dy < MINIMAP_SIZE)
				mlx_put_pixel(g->minimap, cx + dx, cy + dy, 0xFF0000FF);
			dx++;
		}
		dy++;
	}
}
