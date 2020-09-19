#pragma once

#include <GLUT/glut.h>
#include <OpenGL/GL.h>
#include <OpenGL/GLU.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <limits>
#include <tuple>
#include <map>
#include <string>
#include <algorithm>

#include "../environment/Walls.h"
#include "../map/map.h"
#include "../texturing/TextureColours.h"
#include "../texturing/texture.h"
#include "../configuration/ConfigInit.h"
#include "../texturing/TextureLoader.h"

#define _USE_MATH_DEFINES
#include <math.h>

constexpr double THREE_HALF_PI = 3 * M_PI / 2;
constexpr float DR = 0.0174533f;