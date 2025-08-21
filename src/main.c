/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:08:55 by clu               #+#    #+#             */
/*   Updated: 2025/08/18 13:37:05 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Main function to initialize the game through mlx42
** Clean up and exit if initialization fails
** Setup controls and event loops
** Start the main loop
*/
int	main(int argc, char **argv)
{
	t_game	g;

	if (argc != 2)
	{
		printer("Invalid arguments.\nUsage: ./cub3d <map.cub>", NULL);
		return (EXIT_FAILURE);
	}
	if (game_init(&g, argv[1]) != EXIT_SUCCESS)
	{
		printer("Failed to initialize game.", NULL);
		cleanup_game(&g);
		return (EXIT_FAILURE);
	}
	print_controls();
	mlx_loop_hook(g.mlx, handle_input, &g);
	mlx_key_hook(g.mlx, handle_keypress, &g);
	mlx_loop(g.mlx);
	cleanup_game(&g);
	return (EXIT_SUCCESS);
}

/*
** Initialize the structs with defaults
** Parse .cub file and validate the map file
** Set player position based on the parser info
** Initialize mlx42 and create the window
** Load images, textures, and minimap
** Disable mouse cursor to allow rotation control with mouse
*/
int	game_init(t_game *g, char *path)
{
	init_game_struct(g);
	if (!setup_base(g, path))
		return (EXIT_FAILURE);
	g->player.x = g->map_data.start_x + 0.5;
	g->player.y = g->map_data.start_y + 0.5;
	set_p_dir_n_s(g);
	set_p_dir_e_w(g);
	g->mlx = mlx_init(WIN_W, WIN_H, "cub3d", true);
	if (!g->mlx)
	{
		cleanup_game(g);
		return (EXIT_FAILURE);
	}
	if (!load_images(g))
	{
		cleanup_game(g);
		return (EXIT_FAILURE);
	}
	mlx_set_cursor_mode(g->mlx, MLX_MOUSE_DISABLED);
	return (EXIT_SUCCESS);
}

/*
** Loads main images, textures, and minimap for the game
*/
int	load_images(t_game *g)
{
	g->img = mlx_new_image(g->mlx, WIN_W, WIN_H);
	if (!g->img)
		return (0);
	if (!load_textures(g))
		return (0);
	mlx_image_to_window(g->mlx, g->img, 0, 0);
	g->minimap = mlx_new_image(g->mlx, MINIMAP_SIZE, MINIMAP_SIZE);
	if (!g->minimap)
		return (0);
	mlx_image_to_window(g->mlx, g->minimap, MINIMAP_VIEW_H, MINIMAP_VIEW_W);
	return (1);
}

/*
** Initialization of the structs with default values
*/
void	init_game_struct(t_game *g)
{
	ft_bzero(g, sizeof(t_game));
	g->map_data.start_dir = '0';
	g->map_data.floor_color = (uint32_t) - 1;
	g->map_data.ceiling_color = (uint32_t) - 1;
	g->first_mouse = true;
	g->last_mouse_x = WIN_W / 2;
	g->last_mouse_y = WIN_H / 2;
	g->mouse_disabled = true;
}
