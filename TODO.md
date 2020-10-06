# TODO

![TODO Status](https://img.shields.io/badge/TODO-outstanding-yellow?style=for-the-badge&logo=markdown)

* Create generic method for both `checkVertical()` and `checkHorizontal()` called `checkDir()`
* ~~Break `renderRays2Dto3D()` method up into smaller methods and generify where possible~~
* ~~Create texture hashing by string using polynomial rolling~~
* *Change rendering to BMP columns* (WIP)
* ~~Add config files and reader~~
* Implement logging:
  * ~~OpenGL + GLUT ([OpenGL debug wiki](https://www.khronos.org/opengl/wiki/Debug_Output))~~
  * Player position
  * ~~Texture processing~~
  * ~~Map processing~~
* ~~Change renderer to query wall texture~~
* ~~Fix HashTable (change to hashmap K,V pairs) and fix Texture~~
* Implement unit tests:
  * A*
  * BSP
  * BMP read
  * ~~INI read~~
  * ~~JSON read~~
  * map load
  * texture load
  * hash table usage
  * hashing collision rate
* ~~Change map file to JSON and map reader to parse JSON~~
* ~~Add collision detection with walls~~
* ~~Implement A* search algorithm~~
* ~~Add start + end locations in map definitions~~
* Implement BSP search tree based on first and last rays
* Change raycasting to query against walls from results of BSP rather than raymarching
* ~~Implement minmap and player position scaling~~
* Fix ray rendering on minimap
* Fix linux GL window rendering
* Change TextureLoader to iterate through `resources/textures/*` and load all in there (verify existance and format)

## BSP Tree

### Looking along x-axis

* The root node is **ALWAYS** the closest to center point on the grid.
  * Root: $\lparen\lfloor{\frac{width}{2}\rfloor},\lfloor{\frac{height}{2}\rfloor}\rparen$
* Insert nodes based on x coord first (l-r or r-l)
* If coord.x is less then current.x node place on left
* If coord.x is greater than current.x node plane on right
* If coord.x is equal to current.y node:
  * If coord.y is less than current.y node place on left
  * If coord.y is greater than current.y node place on right

Tree building:

* $L\to R$ then $U\to D$ if from left
* $R\to L$ then $U\to D$ if from right
* $U\to D$ then $L\to R$ if from up
* $D\to U$ then $L\to R$ if from down

Only need to build $L\to R$ or $R\to L$ and $U\to D$ or $D\to U$ since reversing traversing sign in tree accounts for other case

* Traversing tree based on origin location relative to node
* Test rays against nodes
  * If ray intersects, take note of node
  * Remove ray from query pool
