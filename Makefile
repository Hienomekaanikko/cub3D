# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clu <clu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/04 13:02:40 by clu               #+#    #+#              #
#    Updated: 2025/08/18 13:23:53 by clu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program Name
NAME = cub3d

# Comiler and Flags
CC 		= cc
CFLAGS 	= -Wall -Werror -Wextra

# Source directories
SRC_DIR 	= ./src/
INIT_DIR	= init/
INPUT_DIR	= input/
RENDER_DIR	= render/
UTILS_DIR	= utils/
PARSING_DIR = parsing/

# Object files directory
OBJ_DIR 	= ./obj

# Library directories
LIBFT_DIR 	= ./libft
MLX_DIR		= ./MLX42

# Libraries
LIBFT	:= $(LIBFT_DIR)/libft.a
LIBMLX	:= $(MLX_DIR)/build/libmlx42.a
LIBS	:= $(LIBFT) $(LIBMLX) -ldl -lglfw -pthread -lm

# Include directories
HEADERS = -I./include -I$(LIBFT_DIR)/include -I$(MLX_DIR)/include/MLX42
HEADERS_DEP := include/cub3d.h

# Source files and object files
# shell runs the command inside it and returns the output
SRC = 	$(SRC_DIR)main.c \
		$(SRC_DIR)$(INIT_DIR)init_game.c \
		$(SRC_DIR)$(INPUT_DIR)input.c \
		$(SRC_DIR)$(INPUT_DIR)move.c \
		$(SRC_DIR)$(RENDER_DIR)draw.c \
		$(SRC_DIR)$(RENDER_DIR)draw_utils.c \
		$(SRC_DIR)$(RENDER_DIR)raycast.c \
		$(SRC_DIR)$(RENDER_DIR)minimap.c \
		$(SRC_DIR)$(RENDER_DIR)minimap_utils.c \
		$(SRC_DIR)$(UTILS_DIR)utils.c \
		$(SRC_DIR)$(PARSING_DIR)parse_base_setup.c \
		$(SRC_DIR)$(PARSING_DIR)parse_colors.c \
		$(SRC_DIR)$(PARSING_DIR)parse_cub_file.c \
		$(SRC_DIR)$(PARSING_DIR)parse_map.c \
		$(SRC_DIR)$(PARSING_DIR)parse_utils.c \
		$(SRC_DIR)$(PARSING_DIR)parse_map_utils.c

OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)/%.o)

# Rules
all: $(LIBMLX) $(LIBFT) $(NAME)

# Build the executable
$(NAME): $(OBJ)
	@echo "Building executable $(NAME) ..."
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(HEADERS) -o $(NAME)
	@echo "Done"

# Compile the object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS_DEP)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# Compile the Libft library
$(LIBFT):
	@echo "Compiling Libft..."
	@make -C $(LIBFT_DIR) -j
	@echo "Done"

# Compile the MLX42 library
$(LIBMLX):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "Cloning MLX42..."; \
		git clone https://github.com/codam-coding-college/MLX42.git $(MLX_DIR); \
	fi
	@echo "Building MLX42..."; \
	cmake $(MLX_DIR) -B $(MLX_DIR)/build; \
	make -C $(MLX_DIR)/build -j;

# Clean up object files
clean:
	@make -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@rm -rf $(MLX_DIR)/build
	@echo "Cleaned up all object files"

# Clean up everything
fclean: clean
	@rm -f $(NAME)
	@rm -rf $(MLX_DIR)
	@make -C $(LIBFT_DIR) fclean
	@echo "Cleaned up $(NAME)"
	@echo "Cleaned up everything"

# Remake everything
re: fclean all
	@echo "Remade everything"

# Phony
.PHONY: all clean fclean re
