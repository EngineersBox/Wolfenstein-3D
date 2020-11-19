#pragma once

#if _WIN64 || _WIN32
    #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES

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
#include <numeric>
#include <map>

#include "../../io/configuration/ConfigInit.cpp"
#include "../drawing/DrawingUtils.hpp"
#include "../drawing/RasterText.hpp"
#include "../../environment/constructs/walls/AABB.cpp"
#include "../../environment/constructs/walls/AABBFace.cpp"
#include "../../environment/world/Coordinates.hpp"
#include "../../environment/world/World.cpp"
#include "../partitioning/QSPTree.cpp"
#include "../../logic/pathfinding/AStar.cpp"
#include "../../environment/player/Player.cpp"
#include "../colour/Colours.cpp"
#include "../../io/resource_management/TextureLoader.cpp"
#include "../texturing/texture.cpp"
#include "../Globals.hpp"
#include "Ray.hpp"
#include "../../io/resource_management/PNGReader.hpp"
#include "../../environment/constructs/sprites/Sprite.cpp"
#include "../buffering/PBO.cpp"
#include "../../gui/minimap/Minimap.cpp"
#include "../../gui/debug_overlay/DebugOverlay.cpp"
#include "../../gui/stats_bar/StatsBar.cpp"
#include "../../gui/Canvas.cpp"