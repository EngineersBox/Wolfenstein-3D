# Wolfenstein 3D

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/EngineersBox/Wolfenstein-3D/CMake?style=for-the-badge) ![TODO Status](https://img.shields.io/badge/TODO-outstanding-yellow?style=for-the-badge&logo=markdown)

A faithful re-creation of Id Software's, Wolfensetin 3D from 1992.

Something important to note about this repo and its implementation. The idea behind this is to use the most low level OpenGL functionality and handle of all of the graphics processing manually. This means not using OpenGL/GLUT texturing managing and perspective polygon rendering + scaling. Instead all of this is calculated as part of the raycasting implementation.

## Creditations

There are some crediations that need to be made for certain pieces of software used or based around in this project:

* Textures used are property of ID Software
* Raycasting based on implementation by Lode Vandevenne at [https://lodev.org/cgtutor/raycasting.html#Textured Raycaster](https://lodev.org/cgtutor/raycasting.html#Textured%sRaycaster)
* PNG Reader attributed to Lode Vandevenne at [https://lodev.org/](https://lodev.org/)
* JSON Reader attributed to Subhrajit Bhattacharya at [http://subhrajit.net/](http://subhrajit.net/)
* INI Reader attributed to Ben Hoyt at [https://github.com/benhoyt/inih](https://github.com/benhoyt/inih)
* Color RGB, Color RGB8bit, Color HSV and Color HSL attributed to Lode Vandevenne at [https://lodev.org/](https://lodev.org/)
All other sources and modications of the above are my own work and attributed as such.

## Building and running

This project is set up in a way that makes it easy to build and run the raycaster. That said, it does differ slightly depending on your system and whether you have cmake installed.

### CMake

If you have CMake installed then running the raycaster is really straight forward:

1. Open up a terminal window and cd to this repo
2. Run `cmake .` to set up the cmake build environment
3. Build the raycaster with `make W3D`
4. Start the built raycaster with `make run`

Below is a full list of commands and their usages:
_NOTE: All commands have supported equivalents in the `makefile.alt` for non CMake users_

| Command                | Description                                                                                                                                          |
| ---------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| `cmake .`              | Initialise the build environment                                                                                                                     |
| `make W3D`             | Build the raycaster to `out/`                                                                                                                        |
| `make run`             | Execute a build in `out/`                                                                                                                            |
| `make run_memprof`     | Execute a build in `out/` within a call of `leaks` _(UNIX only)_                                                                                     |
| `make destroy`         | Removes all files in the `out/`                                                                                                                      |
| `make clean_logs`      | Removes all log files from `logs/`                                                                                                                   |
| `make archive_logs`    | Compress all `*.log` files in `logs/` into a date time named zip archive in the format: `archive-DD-MM-YYYY_HH-MM-SS.zip`                            |
| `make clean_archives`  | Remove all archives from `logs/`                                                                                                                     |
| `make clean_memprof`   | Remove all memory profiles from `memory_analysis/`                                                                                                   |
| `make archive_memprof` | Compres all `*.memgraph` memory profiles from `memory_analysis/` into a date time named zip archive in the format: `archive-DD-MM-YYYY_HH-MM-SS.zip` |
| `make clean_memprof`   | Remove all archives from `memory_analysis/`                                                                                                          |
| `make build_tests`     | Build the test cases to `out/tests`                                                                                                                  |
| `make run_tests`       | Run the test cases with assertion fails only                                                                                                         |
| `make run_tests_wo`    | Run the test cases with output for all assertions                                                                                                    |

### Using alternate makefile

If you don't have CMake installed on your system, you can still build and run the raycaster, you'll just be using a seperate makefile

#### Mac OS X

To build the raycaster:

```bash
make -f Makefile.alt build_osx_clang++
```

_or_

```bash
make -f Makefile.alt build_osx_g++
```

#### Linux

To build the raycaster:

```bash
make -f Makefile.alt build_linux
```

#### Windows

To build the raycaster:

```bash
make -f Makefile.alt build_win
```

### Running the raycaster

To run the built raycaster:

```bash
make -f Makefile.alt run
```

## Test cases

### Testing with CMake

If you have CMake installed then running the test cases is really straight forward:

1. Open up a terminal window and cd to this repo
2. Run `cmake .` to set up the cmake build environment
3. Build the test cases with `make build_tests`
4. Run the test cases with `make run_tests` (_or_ `make run_tests_wo` _if you want to see each of the assertions_)

### Testing with alternate makefile

If you don't have CMake installed on your system, you can still build and run the test cases, you'll just be using a seperate makefile

#### Mac OS X

To build the tests:

```bash
make -f Makefile.alt build_tests_osx_clang++
```

_or_

```bash
make -f Makefile.alt build_tests_osx_g++
```

#### Linux

To build the tests:

```bash
make -f Makefile.alt build_tests_linux
```

#### Windows

To build the tests:

```bash
make -f Makefile.alt build_tests_win
```

### Running the tests

To run the built tests:

```bash
make -f Makefile.alt run_tests
```

Or with assertion output:

```bash
make -f Makefile.alt run_tests_wo
```

---

## Implementation Details

### Map

A map is described by a JSON object contain information about the map parameters and walls. There are three main sections:

- `Params`: Map specific configuration
- `Ceiling`: Specifying the ceiling texture
- `Floor`: Specifying the floor texture
- `Walls`: The layout of the map and how it renders
- `Sprites`: Positions of 2D sprites in the map

Map files are stored in `resources/maps/`. Below is the JSON schema for a map file:

```JSON
{
    "Params": {
        "Name": "<string>",
        "Width": "<int>",
        "Height": "<int>",
        "Start": {
            "x": "<int>",
            "y": "<int>"
        },
        "End": {
            "x": "<int>",
            "y": "<int>"
        }
    },
    "Ceiling": "<string>",
    "Floor": "<string>",
    "Walls": [
        {
            "x": "<int>",
            "y": "<int>",
            "Left": {
                "Texture": "<string>"
            },
            "Right": {
                "Texture": "<string>"
            },
            "Up": {
                "Texture": "<string>"
            },
            "Down": {
                "Texture": "<string>"
            }
        }
    ],
    "Sprites": [
        {
            "x": "<double>",
            "y": "<double>",
            "Texture": "<string>",
            "Enemy": "<bool>",
            "Animation Frames": [
              "<string>"
            ],
            "Tick Rate": "<int>"
        }
    ]
}
```

### INI Config

Configuration for implementation and feature usage is done with a `config.ini` file found in `resources/configs/`.

There are four main sections:

- `player`
- `minimap`
- `logging`
- `rendering`

Below is an example of a configuration file:

```INI
[player]
fov = 80.0
dof = 8.0
move_speed = 3.0
rotation_speed = 1.0

[minimap]
enable = true
render_rays = false
pos = TOP_LEFT
size = MEDIUM

[logging]
gl_debug = true
hide_warnings = false
hide_infos = false
tex_skip_invalid = true
map_skip_invalid = true
log_verbose = true
show_fps = true
show_player_pos = true
show_time_tick = true

[rendering]
headless_mode = false
double_buffer = false
render_walls = true
render_floor_ceiling = true
render_sprites = true
refresh_rate = 60 ; Value in Hz
ray_count = 80
render_distance = 10
texture_width = 64
texture_height = 64

```

## File system tree

In order to make it easier to traverse the file system, here is a tree depicting how the nestings of directories is layed out:

- resources
  - configs
  - maps
  - textures
- src
  - environment
    - base
      - element
      - entity
      - object
    - constructs
      - doors
      - sprites
      - walls
    - player
    - world
  - exceptions
    - buffering
    - configuration
    - debug
    - hashing
    - heap
    - image
    - map
    - pathfinding
    - textures
  - gui
    - base
    - button
    - debug_overlay
    - minimap
    - stats_bar
  - io
    - configuration
      - sections
    - logging
    - resource_management
  - logic
    - hashing
    - id
    - pathfinding
    - queue
  - physics
  - rendering
    - buffering
    - colour
    - drawing
    - partitioning
    - raycaster
    - texturing
    - viewmodel
- test
  - asset_loading
  - framework
  - hashing
  - io
  - pathfinding
  - resources
