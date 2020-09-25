#pragma once

#include <GLUT/glut.h>
#include <OpenGL/GL.h>
#include <OpenGL/GLU.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <tuple>
#include <exception>

#include "../configuration/ConfigInit.h"
#include "../drawing/DrawingUtils.h"
#include "../environment/WallFace.h"
#include "../environment/Walls.h"
#include "../hashing/HashTable.h"
#include "../map/map.h"
#include "../player/Player.h"
#include "../texturing/TextureColours.h"
#include "../texturing/TextureLoader.h"
#include "../texturing/texture.h"
#include "../exceptions/image/PixelColumnInvalidIndex.h"
#include "Ray.h"
#include "Globals.h"

#define _USE_MATH_DEFINES

constexpr double THREE_HALF_PI = 3 * M_PI / 2;
constexpr float DR = 0.0174533f;