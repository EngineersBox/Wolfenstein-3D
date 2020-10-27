# QSPMinimalRendering

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/EngineersBox/BSPMinimalRendering/CMake?style=for-the-badge) ![TODO Status](https://img.shields.io/badge/TODO-outstanding-yellow?style=for-the-badge&logo=markdown)

Finding the minimal amount of polygons to render based with A* and QSP trees.

Something important to note about this repo and its implementation. The idea behind this is to use the most low level OpenGL functionality and handle of all of the graphics processing manually. This means not using OpenGL/GLUT texturing managing and perspective polygon rendering + scaling. Instead all of this is calculated as part of the raycasting implementation.

## Building and running

This project is set up in a way that makes it easy to build and run the raycaster. That said, it does differ slightly depending on your system and whether you have cmake installed.

### CMake

If you have CMake installed then running the raycaster is really straight forward:

1. Open up a terminal window and cd to this repo
2. Run `cmake .` to set up the cmake build environment
3. Build the raycaster with `make QSP`
4. Start the built raycaster with `make run`

Below is a full list of commands and their usages:

|Command|Description|
|---|---|
|`cmake .`| Initialise the build environment|
|`make QSP`| Build the raycaster to `out/`|
|`make run`| Execute a build in `out/`|
|`make destroy`| Removes all files in the `out/`|
|`make clean_logs`| Removes all log files from `logs/`|
|`make archive_logs`| Compress all `*.log` files into a date time named zip archive in the format: `archive-DD-MM-YYYY_HH-MM-SS.zip`|
|`make clean_archives`| Remove all archives from `logs/`|
|`make build_tests`| Build the test cases to `out/tests`|
|`make run_tests`| Run the test cases with assertion fails only|
|`make run_tests`| Run the test cases with output for all assertions|

### Using alternate makefile

If you don't have CMake installed on your system, you can still build and run the raycaster, you'll just be using a seperate makefile

#### Mac OS X

To build the raycaster:

```bash
make -f Makefile.alt build_osx_clang++
```

*or*

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
4. Run the test cases with `make run_tests` (*or* `make run_tests_wo` *if you want to see each of the assertions*)

### Testing with alternate makefile

If you don't have CMake installed on your system, you can still build and run the test cases, you'll just be using a seperate makefile

#### Mac OS X

To build the tests:

```bash
make -f Makefile.alt build_tests_osx_clang++
```

*or*

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

## Involved Algorithms

### Shortest Path

In order to find the shortest path, an extension of Dijkstra's algorithm is used; known as A*. This type of path finding uses a heuristic based on distance relative to both the start and end positions. In this case however, the heuristic is modified to also take into acount polygon count changes at each move.

![https://upload.wikimedia.org/wikipedia/commons/5/5d/Astar_progress_animation.gif](https://upload.wikimedia.org/wikipedia/commons/5/5d/Astar_progress_animation.gif)

### Raycasting with QSP Trees

Rendering of the scene is done via a raycasting algorithm. This renders polygons based on rays cast outward from the players perspective, then renders a polygon based on the starting and ending positions determined from the rays. In order to optimise this process, binary space partitioning is used.

![https://upload.wikimedia.org/wikipedia/commons/8/81/Binary_space_partition.png](https://upload.wikimedia.org/wikipedia/commons/8/81/Binary_space_partition.png)

This process involes quatering the render space convex hull into smaller convex hulls based on relative direction between parent and child nodes. Once the render space is divided into minmal convex hulls, a render convex hull is formed that includes the minimal amount of polygons to render to fill the entire screen. This improves performance since polygons can be determined to render infront of others without having to query against them in a traditional raycaster

![https://upload.wikimedia.org/wikipedia/commons/thumb/0/08/Example_of_BSP_tree_traversal.svg/1920px-Example_of_BSP_tree_traversal.svg.png](https://upload.wikimedia.org/wikipedia/commons/thumb/0/08/Example_of_BSP_tree_traversal.svg/1920px-Example_of_BSP_tree_traversal.svg.png)

#### Links:

* [OpenGL BSP implementation](https://www.opengl.org/archives/resources/code/samples/bspfaq/)

### Texture hashing

Reducing the overhead of texture references is done with custom texture hashing. Since the textures used are simple and have deterministic object structure a hash can be produced based on a rolling polynomial string hashing method. For the name of the texture. There will also be an alternative method used for the byte array describing the texture object.

![https://upload.wikimedia.org/wikipedia/commons/thumb/5/58/Hash_table_4_1_1_0_0_1_0_LL.svg/240px-Hash_table_4_1_1_0_0_1_0_LL.svg.png](https://upload.wikimedia.org/wikipedia/commons/thumb/5/58/Hash_table_4_1_1_0_0_1_0_LL.svg/240px-Hash_table_4_1_1_0_0_1_0_LL.svg.png)

---

## Implementation Details

### Map

A map is described by a JSON object contain information about the map parameters and walls. There are two main sections:

* `Params`
* `Walls`

Map files are stored in `resources/maps/`. Below is the JSON type data and structure of the file:

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
    "Walls": [
        {
            "x": "<int>",
            "y": "<int>",
            "Left": {
                "Colour": "<colour>",
                "Texture": "<string>"
            },
            "Right": {
                "Colour": "<colour>",
                "Texture": "<string>"
            },
            "Up": {
                "Colour": "<colour>",
                "Texture": "<string>"
            },
            "Down": {
                "Colour": "<colour>",
                "Texture": "<string>"
            }
        }
    ]
}
```

### INI Config

Configuration for implementation and feature usage is done with a `config.ini` file found in `resources/configs/`.

There are four main sections:

* `player`
* `minimap`
* `logging`
* `rendering`

Below is an example of a configuration file:

```INI
[player]
fov = 80.0
dof = 8.0
sensitivity = 2.0

[minimap]
enable = true
render_rays = false ; NOT YET IMPLEMENTED
pos = "TOP_RIGHT"
size = "MEDIUM"

[logging]
gl_debug = true
player_pos = false
hide_warnings = false ; NOT YET IMPLEMENTED
hide_infos = false ; NOT YET IMPLEMENTED
tex_skip_invalid = true
map_skip_invalid = true
log_verbose = true

[rendering] ; NOT YET IMPLEMENTED (ALL)
headless_mode = false
double_buffer = false
refresh_rate = 60 ; Value in Hz
ray_count=80
```
