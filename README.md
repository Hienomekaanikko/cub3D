# cub3D

A raycasting-based 3D game engine written in C, inspired by the rendering technique used in Wolfenstein 3D (1992). The map is a 2D grid, but by casting a ray for every vertical column of the screen and projecting wall heights from the intersection distances, the engine produces a convincing first-person 3D view.

Built with **MLX42** for windowing and pixel rendering.

![Screenshot](https://github.com/user-attachments/assets/22431b9d-9220-4795-9d0b-2906ec03e333)

---

## Usage

```
./cub3d <map.cub>
```

```
./cub3d maps/basic.cub
```

### Controls

| Input | Action |
|-------|--------|
| `W A S D` | Move forward / left / backward / right |
| `← →` | Rotate view left / right |
| Mouse | Rotate view (TAB to toggle mouse capture) |
| `ESC` | Quit |

---

## Map Format (`.cub`)

A `.cub` file defines all scene elements: textures, colours, and the map grid.

```
NO textures/winter_forest.png
SO textures/winter_forest_2.png
WE textures/winter_forest_3.png
EA textures/winter_forest_4.png

F 255,255,255
C 135,206,235

1111111111111
1000000000001
1000100010001
100000S000000001
1111111111111
```

| Element | Description |
|---------|-------------|
| `NO / SO / WE / EA` | PNG texture paths for each cardinal wall face |
| `F r,g,b` | Floor colour |
| `C r,g,b` | Ceiling colour |
| `1` | Wall tile |
| `0` | Open floor tile |
| `N / S / E / W` | Player spawn position and starting direction |

**Validation rules enforced by the parser:**
- All textures must be readable files (not directories)
- Floor and ceiling colours must be valid RGB values
- Each identifier may only appear once
- The map must be fully enclosed by walls (flood-fill check from the spawn point)
- No illegal characters or empty lines within the map body
- Exactly one player spawn marker

---

## How the Raycasting Works

### 1. Camera Plane

The player has a direction vector `(dir_x, dir_y)` and a perpendicular camera plane `(plane_x, plane_y)`. The FOV is determined by the ratio of the plane length to the direction length (`FOV = 0.7`, giving ~70°).

### 2. Ray Casting — one ray per screen column

For each column `x` across the 1920-pixel-wide screen, a ray is computed:

```c
camera_x = 2 * x / WIN_W - 1;          // -1 (left edge) to +1 (right edge)
ray.dir_x = player.dir_x + plane_x * camera_x;
ray.dir_y = player.dir_y + plane_y * camera_x;
```

### 3. DDA — Digital Differential Analysis

The DDA algorithm steps the ray through the 2D grid cell by cell, always advancing along whichever axis brings it to the next grid line sooner. It tracks two accumulators:

- `side_x` — distance the ray must travel to cross the next vertical grid line
- `side_y` — distance to the next horizontal grid line

At each step, whichever is smaller is advanced by its `delta` (the distance between parallel grid lines in that direction). When the ray lands on a wall tile (`'1'`), the loop stops and the **perpendicular wall distance** is computed — corrected to avoid the fisheye distortion that straight Euclidean distance would cause.

### 4. Wall Projection

The perpendicular distance determines the height of the wall slice drawn on screen:

```
line_height = WIN_H / perp_wall
draw_start  = WIN_H / 2 - line_height / 2
draw_end    = WIN_H / 2 + line_height / 2
```

Close walls produce tall slices; far walls produce short ones. Ceiling and floor pixels are filled with the flat colours defined in the `.cub` file.

### 5. Texture Mapping

The wall face hit (N/S/E/W) is determined from the ray direction and which axis (`side == 0` → E/W, `side == 1` → N/S) was last crossed. The exact horizontal position within the wall tile is then used to select a column `tex_x` from the appropriate PNG texture, and the texture is sampled vertically at a step rate proportional to `texture_height / line_height` to scale it correctly onto the wall slice.

### 6. Distance Fog

Walls dim as they recede using an inverse-distance formula:

```c
intensity = 1.0 / (1.0 + distance * 0.15)   // clamped to [0.2, 1.0]
```

Each RGB channel is multiplied by this intensity, giving a natural depth cue without any post-processing pass.

---

## Minimap

A 300×300 pixel overlay renders a top-down view centred on the player. It shows a 20×20 tile viewport that scrolls with the player, with white tiles for walls, dark grey for floor, a dot for the player position, and a light-blue direction line showing where the player is facing.

---

## Movement

Movement is frame-rate independent: speed is scaled by `mlx->delta_time` so the player moves the same distance per second regardless of frame rate. Collision detection is axis-split — X and Y are tested independently against the map grid, so the player slides along walls instead of stopping dead.

Mouse look is handled by reading raw delta from `mlx_get_mouse_pos` each frame and applying a rotation matrix to the direction and camera plane vectors:

```c
dir_x' = dir_x * cos(rot) - dir_y * sin(rot)
dir_y' = dir_x * sin(rot) + dir_y * cos(rot)
```

---

## Structure

```
cub3d/
├── src/
│   ├── main.c               Entry point, game init, MLX42 loop setup
│   ├── init/
│   │   └── init_game.c      Texture loading, player direction init
│   ├── parsing/
│   │   ├── parse_cub_file.c Top-level .cub parser, map validation
│   │   ├── parse_colors.c   F/C colour parsing
│   │   ├── parse_map.c      Grid extraction and closed-map flood-fill
│   │   └── parse_utils.c    Helpers: texture path extraction, error codes
│   ├── render/
│   │   ├── raycast.c        Ray init, DDA loop, wall hit detection
│   │   ├── draw.c           Wall slice rendering, texture sampling
│   │   ├── draw_utils.c     Wall bounds, texture selection, distance fog
│   │   ├── minimap.c        Minimap rendering, player dot, direction line
│   │   └── minimap_utils.c  Tile drawing helpers
│   ├── input/
│   │   ├── input.c          Input hook, per-frame handler dispatch
│   │   └── move.c           WASD movement, arrow/mouse rotation
│   └── utils/
│       └── utils.c          Cleanup, texture pixel sampling, control print
├── include/
│   └── cub3d.h              All structs, constants, and function declarations
├── maps/                    Sample .cub map files
├── textures/                Wall texture PNGs
└── libft/                   Custom C standard library
```

---

## Building

```
make        # builds ./cub3d
make re     # rebuild from scratch
make fclean # remove objects and binary
```

Requires **MLX42** to be installed (the Makefile links against it). On macOS, also links against `-lm` and `-framework OpenGL`.
