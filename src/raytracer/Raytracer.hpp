#pragma once

#if _WIN64 || _WIN32
    #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <exception>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "../configuration/ConfigInit.hpp"
#include "../drawing/DrawingUtils.hpp"
#include "../environment/AABB.hpp"
#include "../environment/AABBFace.hpp"
#include "../exceptions/image/PixelColumnInvalidIndex.hpp"
#include "../hashing/HashTable.hpp"
#include "../map/Coordinates.hpp"
#include "../map/map.hpp"
#include "../partitioning/QSPTree.hpp"
#include "../pathfinding/AStar.hpp"
#include "../player/Player.hpp"
#include "../texturing/TextureColours.hpp"
#include "../texturing/TextureLoader.hpp"
#include "../texturing/texture.hpp"
#include "../world/World.hpp"
#include "Globals.hpp"
#include "Ray.hpp"
#include "../io/PNGReader.hpp"

#define _USE_MATH_DEFINES
constexpr float DR = 0.0174533f;