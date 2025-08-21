/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:05:06 by clu               #+#    #+#             */
/*   Updated: 2025/08/12 17:20:31 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Print out instruction for the controls in the terminal
*/
void	print_controls(void)
{
	ft_printf("\033[1;36m╔═══════════════════════════════╗\033[0m\n");
	ft_printf("\033[1;36m║\033[1;33m        CUB3D CONTROLS         "
		"\033[1;36m║\033[0m\n");
	ft_printf("\033[1;36m╚═══════════════════════════════╝\033[0m\n");
	ft_printf("\033[1;35m🎮 Movement:\033[0m\n");
	ft_printf("  \033[1;32m▶ W/A/S/D\033[0m - Move forward/left/backward"
		"/right\n");
	ft_printf("  \033[1;32m▶ ←/→\033[0m     - Look left/right\n");
	ft_printf("  \033[1;32m▶ Mouse\033[0m   - Look around (when enabled)\n");
	ft_printf("\n\033[1;94m⚙️  Controls:\033[0m\n");
	ft_printf("  \033[1;93m▶ TAB\033[0m     - Toggle mouse cursor\n");
	ft_printf("  \033[1;91m▶ ESC\033[0m     - Exit game\n");
	ft_printf("\n\033[2;37m💡 Tip: Use mouse for smoother camera movement!"
		"\033[0m\n");
	ft_printf("\033[1;36m═══════════════════════════════════════\033[0m\n\n");
}

/*
** Clean up the image textures
*/
void	cleanup_textures(t_game *g)
{
	int	i;

	if (!g || !g->mlx)
		return ;
	i = 0;
	while (i < 4)
	{
		if (g->textures[i])
		{
			mlx_delete_image(g->mlx, g->textures[i]);
			g->textures[i] = NULL;
		}
		i++;
	}
}

/*
** Clean up game struct
*/
void	cleanup_game(t_game *g)
{
	if (!g)
		return ;
	if (g->mlx)
	{
		cleanup_textures(g);
		if (g->img)
			mlx_delete_image(g->mlx, g->img);
		if (g->minimap)
			mlx_delete_image(g->mlx, g->minimap);
		mlx_terminate(g->mlx);
	}
	free_stuff(g);
}

/*
** Extracting the pixel's RGBA colour from a texture
*/
uint32_t	get_texture_pixel(mlx_image_t *tex, int x, int y)
{
	uint8_t	*pixel;

	pixel = &tex->pixels[(y * tex->width + x) * 4];
	return ((pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3]);
}
