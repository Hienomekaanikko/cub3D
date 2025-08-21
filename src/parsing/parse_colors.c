/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:38:17 by msuokas           #+#    #+#             */
/*   Updated: 2025/08/18 13:29:52 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//makes sure that rgb value is set between 0 and 255
static int	is_valid_rgb(char *str, char **colors)
{
	int	val;

	val = ft_atoi(str);
	if (val >= 0 && val <= 255)
		return (1);
	ft_free_split(colors);
	return (0);
}

static int	invalid_color_string(char **colors)
{
	int	i;
	int	j;
	int	error;

	i = 0;
	j = 0;
	error = 0;
	while (colors[i] && error == 0)
	{
		j = 0;
		if (colors[i][j] == '-')
			j++;
		while (colors[i][j] && error == 0)
		{
			if (!ft_isdigit(colors[i][j]) && colors[i][j] != '\n')
				error = 1;
			j++;
		}
		if (ft_strchr(colors[i], ' ') || colors[i][0] == '\n' || !colors[i][0])
			error = 1;
		i++;
	}
	if (error == 1)
		ft_free_split(colors);
	return (error);
}

//counts that there are 3 values for rgb
static int	count_colors(char **colors)
{
	int	count;

	count = 0;
	while (colors[count])
		count++;
	if (count != 3)
	{
		ft_free_split(colors);
		return (0);
	}
	return (1);
}

//validates that the colors are in correct format and range
static int	validate_rgb(uint32_t *color, char *line, int i)
{
	char	**colors;
	int		r;
	int		g;
	int		b;

	if (line[1] != ' ' || has_illegal_spaces(line))
		return (-3);
	while (line[i] == ' ')
		i++;
	colors = ft_split(&line[i], ',');
	if (!colors)
		return (-1);
	if (count_colors(colors) == 0 || invalid_color_string(colors))
		return (-3);
	if (!is_valid_rgb(colors[0], colors) || !is_valid_rgb(colors[1], colors)
		|| !is_valid_rgb(colors[2], colors))
		return (-4);
	r = ft_atoi(colors[0]);
	g = ft_atoi(colors[1]);
	b = ft_atoi(colors[2]);
	*color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
	ft_free_split(colors);
	return (1);
}

//sets F, C to their place
void	place_color(t_game *g, char *line, int i, int *status)
{
	if (line[i] == 'F')
	{
		if (!g->map_data.floor_color_set)
		{
			i++;
			*status = validate_rgb(&g->map_data.floor_color, line, i);
			if (*status == 1)
				g->map_data.floor_color_set = 1;
		}
		else
			*status = -7;
	}
	else if (line[i] == 'C')
	{
		if (!g->map_data.ceiling_color_set)
		{
			i++;
			*status = validate_rgb(&g->map_data.ceiling_color, line, i);
			if (*status == 1)
				g->map_data.ceiling_color_set = 1;
		}
		else
			*status = -8;
	}
}
