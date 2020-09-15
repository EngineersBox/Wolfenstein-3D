# BSPMinimalRendering
Finding the minimal amount of polygons to render based with A* and BSP trees.

## Shortest Path
In order to find the shortest path, an extension of Dijkstra's algorithm is used; known as A*. This type of path finding uses a heuristic based on distance relative to both the start and end positions. In this case however, the heuristic is modified to also take into acount polygon count changes at each move.

![https://upload.wikimedia.org/wikipedia/commons/5/5d/Astar_progress_animation.gif](https://upload.wikimedia.org/wikipedia/commons/5/5d/Astar_progress_animation.gif)

## Raycasting with BSP Trees
Rendering of the scene is done via a raycasting algorithm. This renders polygons based on rays cast outward from the players perspective, then renders a polygon based on the starting and ending positions determined from the rays. In order to optimise this process, binary space partitioning is used.

![https://upload.wikimedia.org/wikipedia/commons/8/81/Binary_space_partition.png](https://upload.wikimedia.org/wikipedia/commons/8/81/Binary_space_partition.png)

This process involes havling the render space convex hull into smaller convex hulls. Once the render space is divided into minmal convex hulls, a render convex hull is formed that includes the minimal amount of polygons to render to fill the entire screen. This improves performance since polygons can be determined to render infront of others without having to query against them in a traditional raycaster

![https://upload.wikimedia.org/wikipedia/commons/thumb/0/08/Example_of_BSP_tree_traversal.svg/1920px-Example_of_BSP_tree_traversal.svg.png](https://upload.wikimedia.org/wikipedia/commons/thumb/0/08/Example_of_BSP_tree_traversal.svg/1920px-Example_of_BSP_tree_traversal.svg.png)

### Links:
* [OpenGL BSP implementation](https://www.opengl.org/archives/resources/code/samples/bspfaq/)

## Texture hashing
Reducing the overhead of texture references is done with custom texture hashing. Since the textures used are simple and have deterministic object structure a hash can be produced based on a rolling polynomial string hashing method. For the name of the texture. There will also be an alternative method used for the byte array describing the texture object.

![https://upload.wikimedia.org/wikipedia/commons/thumb/5/58/Hash_table_4_1_1_0_0_1_0_LL.svg/240px-Hash_table_4_1_1_0_0_1_0_LL.svg.png](https://upload.wikimedia.org/wikipedia/commons/thumb/5/58/Hash_table_4_1_1_0_0_1_0_LL.svg/240px-Hash_table_4_1_1_0_0_1_0_LL.svg.png)
