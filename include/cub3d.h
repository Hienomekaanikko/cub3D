/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:02:42 by clu               #+#    #+#             */
/*   Updated: 2025/08/18 14:37:57 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "MLX42.h"
# include "libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>

# define WIN_W 		1920
# define WIN_H 		1440
# define FOV 		0.7
# define MOVE_UNIT	3.0
# define ROT_RADIAN	1.5

# define NORTH	0
# define SOUTH	1
# define EAST	2
# define WEST	3

# define MINIMAP_SIZE	300
# define MINIMAP_TILE	15
# define MINIMAP_VIEW_W	20
# define MINIMAP_VIEW_H	20
# define MINIMAP_LINE	12

typedef struct s_player
{
	double		x;
	double		y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}	t_player;

typedef struct s_wall_texture
{
	int			tex_x;
	double		wall_x;
	double		step;
	double		tex_pos;
}	t_wall_texture;

typedef struct s_draw_bounds
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			wall_dir;
}	t_draw_bounds;

typedef struct s_minimap_view
{
	int		start_x;
	int		start_y;
	double	px;
	double	py;
	int		x;
	int		y;
	double	line_start_x;
	double	line_start_y;
	double	line_end_x;
	double	line_end_y;
	double	line_current_x;
	double	line_current_y;
	double	line_inc_x;
	double	line_inc_y;
	int		line_steps;
}	t_minimap_view;

typedef struct s_ray
{
	double	dir_x;
	double	dir_y;
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	double	perp_wall;
	int		step_x;
	int		step_y;
	int		side;
	int		map_x;
	int		map_y;
}	t_ray;

typedef struct s_map_data
{
	char		**grid;
	int			width;
	int			height;
	char		*no_tex;
	char		*so_tex;
	char		*we_tex;
	char		*ea_tex;
	uint32_t	floor_color;
	uint32_t	ceiling_color;
	int			floor_color_set;
	int			ceiling_color_set;
	double		start_x;
	double		start_y;
	char		start_dir;
}	t_map_data;

typedef struct s_game
{
	mlx_t			*mlx;
	t_map_data		map_data;
	t_player		player;
	t_ray			ray;
	mlx_image_t		*img;
	mlx_image_t		*minimap;
	mlx_image_t		*textures[4];
	t_wall_texture	tex;
	t_draw_bounds	bounds;
	char			**map;
	int				width;
	int				height;
	int				mouse_x;
	int				mouse_y;
	int				last_mouse_x;
	int				last_mouse_y;
	bool			first_mouse;
	bool			mouse_disabled;
}	t_game;

// Parsing //
void		place_color(t_game *game, char *line, int i, int *status);
int			parse_cub_file(t_game *game, int fd, char **temp_map);
void		error(t_game *game, int *err_code, char *path);
void		printer(char *msg, char *identifyer);
void		file_guide(void);
void		handle_line_data(t_game *game, char *line, int *status);
void		free_stuff(t_game *game);
int			setup_base(t_game *game, char *path);
int			check_map_validity(char *temp_map, int status);
int			set_grid(t_game *game, char *temp_map, int status);
int			check_map_is_closed(char **cpy, t_map_data m_data, int x, int y);
int			ready_for_map(t_game *game, int *status, char *line, int i);
int			elements_outside(char **grid_cpy);
void		empty_line_midmap(char *line, int *status, char *joined);
int			is_directory(char *cmd);
int			has_illegal_spaces(char *line);

// Initializing and rendering //
// main.c
int			game_init(t_game *g, char *path);
int			load_images(t_game *g);
void		init_game_struct(t_game *g);

// init.c
int			load_textures(t_game *g);
int			load_a_texture(t_game *g, const char *path, int index);
void		set_p_dir_n_s(t_game *g);
void		set_p_dir_e_w(t_game *g);

// input.c
void		handle_input(void *param);
void		handle_keypress(mlx_key_data_t keydata, void *param);
int			is_valid_pos(t_game *g, double x, double y);

// move.c
void		apply_movement(t_game *g, double dir_x, double dir_y);
void		move_p(t_game *g);
void		rotate_p(t_game *g);
void		init_mouse_position(t_game *g);
void		handle_mouse(t_game *g);

// raycast.c
void		cast_rays(t_game *g);
void		init_ray(t_game *g, t_ray *ray, int x);
void		setup_ray_direction(t_game *g, t_ray *ray, int x);
int			perform_dda(t_game *g, t_ray *ray);
int			check_ray_hit(t_game *g, t_ray *ray);

// draw.c
void		setup_wall_texture(t_game *g, t_ray *ray, mlx_image_t *texture);
void		draw_background(t_game *g, int x, int draw_start, int draw_end);
void		draw_textured_wall(t_game *g, int x, t_ray *ray);
void		draw_wall_slice(t_game *g, int x, t_ray *ray);

// draw_utils.c
void		calc_wall_bound(t_ray *ray, int *line_height, int *draw_start,
				int *draw_end);
int			get_wall_textures(t_ray *ray);
uint32_t	apply_distance_fog(uint32_t color, double distance);
double		calc_fog_intensity(double distance);

// minimap.c
void		render_minimap(t_game *g);
void		get_minimap_view(const t_game *g, t_minimap_view *view);
void		draw_map_cell_view(t_game *g, t_minimap_view *view);
void		draw_mini_dir(t_game *g, t_minimap_view *view, int sx, int sy);
void		init_direction_line(t_minimap_view *view, int sx, int sy,
				t_player *player);			

/* minimap_utils.c */
void		draw_minimap_tile(mlx_image_t *img, int px, int py, uint32_t color);
void		draw_mini_p(t_game *g, int cx, int cy);

// utils.c
void		print_controls(void);
void		cleanup_textures(t_game *g);
void		cleanup_game(t_game *g);
uint32_t	get_texture_pixel(mlx_image_t *texture, int x, int y);

#endif
