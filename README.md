# BSPMinimalRendering

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/EngineersBox/BSPMinimalRendering/CMake?style=for-the-badge)
![TODO Status](https://img.shields.io/badge/TODO-outstanding-yellow?style=for-the-badge&logo=appveyor)

Finding the minimal amount of polygons to render based with A* and BSP trees.

## Involved Algorithms

### Shortest Path

In order to find the shortest path, an extension of Dijkstra's algorithm is used; known as A*. This type of path finding uses a heuristic based on distance relative to both the start and end positions. In this case however, the heuristic is modified to also take into acount polygon count changes at each move.

![https://upload.wikimedia.org/wikipedia/commons/5/5d/Astar_progress_animation.gif](https://upload.wikimedia.org/wikipedia/commons/5/5d/Astar_progress_animation.gif)

### Raycasting with BSP Trees

Rendering of the scene is done via a raycasting algorithm. This renders polygons based on rays cast outward from the players perspective, then renders a polygon based on the starting and ending positions determined from the rays. In order to optimise this process, binary space partitioning is used.

![https://upload.wikimedia.org/wikipedia/commons/8/81/Binary_space_partition.png](https://upload.wikimedia.org/wikipedia/commons/8/81/Binary_space_partition.png)

This process involes havling the render space convex hull into smaller convex hulls. Once the render space is divided into minmal convex hulls, a render convex hull is formed that includes the minimal amount of polygons to render to fill the entire screen. This improves performance since polygons can be determined to render infront of others without having to query against them in a traditional raycaster

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
pos = "TOP_RIGHT" ; NOT YET IMPLEMENTED

[logging] ; NOT YET IMPLEMENTED (ALL)
gl_debug = true
player_pos = false
hide_warnings = false
hide_infos = false
tex_skip_invalid = true
map_skip_invalid = true

[rendering] ; NOT YET IMPLEMENTED (ALL)
headless_mode = false
double_buffer = false
refresh_rate = 60 ; Value in Hz
```

