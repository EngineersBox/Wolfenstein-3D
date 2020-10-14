#pragma once

#include <sys/types.h>

using namespace std;

// TODO: Use the below directives to change interaction detection to raytrace based
// instead of purely query based

// DEF: Use raytraced collisions
#ifdef INTER_RAYTRACE_COLLISION
    #define COLLLISION_TYPE 0
#endif
// DEF: Use query based collisions
#ifndef INTER_RAYTRACE_COLLISION
    #define COLLLISION_TYPE 1
#endif

enum INTERACTION_TYPE : size_t {
    BLOCK_ALL = 0,
    PLAYER_ONLY = 1,
    ALL_ENTITIES = 2,
    SOLID_ONLY = 4
};