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

#include "../../io/configuration/ConfigInit.hpp"
#include "../drawing/DrawingUtils.hpp"
#include "../../environment/constructs/walls/AABB.hpp"
#include "../../environment/constructs/walls/AABBFace.hpp"
#include "../../logic/hashing/HashTable.hpp"
#include "../../environment/map/Coordinates.hpp"
#include "../../environment/map/map.hpp"
#include "../partitioning/QSPTree.hpp"
#include "../../logic/pathfinding/AStar.hpp"
#include "../../environment/player/Player.hpp"
#include "../texturing/TextureColours.hpp"
#include "../../io/resource_management/TextureLoader.hpp"
#include "../texturing/texture.hpp"
#include "../../environment/world/World.hpp"
#include "../Globals.hpp"
#include "Ray.hpp"
#include "../../io/resource_management/PNGReader.hpp"
#include "../../environment/constructs/sprites/Sprite.hpp"

#define _USE_MATH_DEFINES
constexpr float DR = 0.0174533f;