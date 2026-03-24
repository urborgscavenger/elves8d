*This project has been created as part of the 42 curriculum by mbauer and wilisson.*

## Description

Cub3D is a minimalist first-person renderer inspired by Wolfenstein 3D. It loads a `.cub` configuration, validates the map, and renders textured walls with ray casting via the MLX42 graphics library. The project focuses on map parsing, player/camera setup, and real-time rendering with a top-down minimap for orientation.

Key features:
- Validated `.cub` maps with textures for each wall orientation (NO, SO, WE, EA) plus floor/ceiling RGB colors
- Single player spawn (N/S/E/W) with directional camera plane and collision-aware movement
- Textured wall rendering with per-ray DDA traversal and strip drawing at 1024x768
- Always-on minimap with player marker and sampled rays for quick spatial awareness
- Graceful error handling for malformed configs, missing assets, or non-closed maps

## Instructions

### Prerequisites
- Linux or macOS with a C toolchain (`cc`, `make`)
- CMake (for the bundled MLX42 build step)
- GLFW/OpenGL development headers (MLX42 dependency). On Debian/Ubuntu: `sudo apt-get install cmake libglfw3-dev libgl1-mesa-dev xorg-dev`.

### Build
From the project root, run:
```
make
```
This will:
- Build libft
- Clone and build MLX42 locally (if absent)
- Compile the `cub3d` executable with the provided sources

Useful targets:
- `make clean` — remove object files
- `make fclean` — full cleanup (binary, objects, MLX42 build)
- `make re` — rebuild from scratch

### Run
After building:
```
./cub3d map.cub
```
Example maps are provided (e.g., `map.cub`, `map_ring_ride.cub`).

### Controls
- Move: `W`/`S` (forward/back), `A`/`D` (strafe)
- Rotate: Left/Right arrows
- Exit: `Esc`

### Map Format
A valid `.cub` file contains texture paths, colors, then the map grid:
```
NO ./path_to_north_texture.png
SO ./path_to_south_texture.png
WE ./path_to_west_texture.png
EA ./path_to_east_texture.png
F 220,100,0
C 225,30,0

111111
1000N1
111111
```
Rules:
- One player start (`N`, `S`, `E`, or `W`) only; it is converted to floor for rendering
- Map must be closed by walls (`1`) with walkable floor cells as `0`; spaces are treated as void (unless it touches 0 = treated as not closed map)
- Floor `F` and ceiling `C` colors use `R,G,B` (0–255)
- Tabs are normalized to spaces; non-map trailing rows are rejected

## Resources

Classic references:
- MLX42: https://github.com/codam-coding-college/MLX42
- LodeV Raycasting Tutorial: https://lodev.org/cgtutor/raycasting.html
- Wolfenstein 3D rendering notes: https://fabiensanglard.net/gebbwolf3d
- MiniLibX/42 graphics primer: https://harm-smits.github.io/42docs/libs/minilibx
- More videos about raycasting: https://www.youtube.com/watch?v=gYRrGTC7GtA&list=WL&index=7
- Fisheye effect: https://gamedev.stackexchange.com/questions/45295/raycasting-fisheye-effect-question
- and some more

AI usage:
AI tools were used as an assistant throughout the development process. All
suggestions were reviewed, adapted, and integrated by the project authors; AI
was not used to produce finished work without human verification.

Specifically, AI helped with:
- Research and learning: concise explanations of raycasting, DDA traversal,
	texture mapping, and MLX42 usage to accelerate implementation decisions.
- Design and architecture of the maps.
- Debugging and troubleshooting: suggesting likely causes and fixes for
	parsing errors, memory issues, and texture-loading failures encountered
	during development.
- Test case generation: recommending edge cases and manual test scenarios for
	map validation, color parsing, and player positioning to harden the parser.
- Documentation and examples: drafting README sections, usage examples, and
	concise error messages to improve usability.
- Productivity: providing small snippets, refactor suggestions, and commit
	message templates to speed up iterative development.