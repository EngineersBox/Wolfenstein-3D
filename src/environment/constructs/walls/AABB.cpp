#pragma once

#include <cmath>
#include <string>
#include <algorithm>

#include "../../../rendering/colour/Colours.cpp"
#include "../../../rendering/texturing/texture.cpp"
#include "AABBFace.cpp"
#include "../../../rendering/raycaster/Ray.hpp"
#include "../../base/object/IObjectBase.cpp"
#include "../../world/Coordinates.hpp"

using namespace std;

namespace Constructs {

enum NormalDir {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

static const string NormalDirLUT[] = {
    "LEFT",
    "RIGHT",
    "UP",
    "DOWN"
};

typedef enum E_WallType {
    NONE,
    WALL,
    DOOR
} WallType;

WallType parseWallType(string wall_type_string) {
    transform(wall_type_string.begin(), wall_type_string.end(),
        wall_type_string.begin(), ::toupper);
    if (wall_type_string == "WALL")  {
        return WallType::WALL;
    } else if (wall_type_string == "DOOR") {
        return WallType::DOOR;
    }
    return WallType::NONE;
}

class AABB : public BaseInterface::IObjectBase<int, Coordinates> {
   public:
    AABB(int x, int y, Colour::RGB colour, string texture, WallType type);
    AABB(int x, int y, Colour::RGB colour, WallType type);
    AABB(int x, int y, string texture, WallType type);
    AABB(int x, int y, Colour::RGB colour, AABBFace wf_left, AABBFace wf_right, AABBFace wf_up, AABBFace wf_down, WallType type);
    AABB();

    bool operator==(AABB& other);
    bool operator!=(AABB& other);
    NormalDir getNormDir(int x, int y);
    AABBFace getFace(NormalDir normDir);
    AABBFace getFace(int x, int y);
    bool intersect(const Ray& ray, Coords& intersect_point);

    Colour::RGB texColour;
    string texture_name;

    AABBFace wf_left;
    AABBFace wf_right;
    AABBFace wf_up;
    AABBFace wf_down;

    WallType type;
};

///
/// AABB object that stores a location and colour
///
/// @param int x: X-axis location
/// @param int y: Y-axis location
/// @param Colour colour: A colour to set as the AABB
/// @param string texture: Name of texture for the AABB
///
/// @returns AABB
///
AABB::AABB(int x, int y, Colour::RGB colour, string texture, WallType type):
    IObjectBase<int, Coordinates>(
        Coordinates<int>(x,y),
        INTERACTION_TYPE::BLOCK_ALL
    )
{
    this->texColour = colour;
    this->texture_name = texture;
    this->type = type;
}

AABB::AABB(int x, int y, Colour::RGB colour, AABBFace wf_left, AABBFace wf_right, AABBFace wf_up, AABBFace wf_down, WallType type):
    IObjectBase<int, Coordinates>(
        Coordinates<int>(x,y),
        INTERACTION_TYPE::BLOCK_ALL
    )
{
    this->texColour = colour;
    this->wf_left = wf_left;
    this->wf_right = wf_right;
    this->wf_up = wf_up;
    this->wf_down = wf_down;
    this->type = type;
};

AABB::AABB() : AABB(0, 0, Colour::RGB_None, "", WallType::NONE) {};

AABB::AABB(int x, int y, string texture, WallType type) : AABB(x, y, Colour::RGB_None, texture, type) {};

AABB::AABB(int x, int y, Colour::RGB colour, WallType type) : AABB(x, y, colour, "", type) {};

bool AABB::operator==(AABB& other) {
    return (this->location.x == other.location.x)
        && (this->location.y == other.location.y)
        && (this->texColour == other.texColour)
        && (this->texture_name == other.texture_name);
};

bool AABB::operator!=(AABB& other) {
    return (this->location.x != other.location.x)
        && (this->location.y != other.location.y)
        && (this->texColour != other.texColour)
        && (this->texture_name != other.texture_name);
};

NormalDir AABB::getNormDir(int x, int y) {
    // cout << to_string(x) << " " << to_string(y) << endl;
    // cout << to_string(this->location.x) << " " << to_string(this->location.y) << endl;
    if ((x == this->location.x || x == this->location.x + 1) && y == this->location.y) {
        return NormalDir::DOWN;
    } else if ((x == this->location.x || x == this->location.x + 1) && y == this->location.y + 1) {
        return NormalDir::UP;
    } else if ((y == this->location.y || y == this->location.y + 1) && x == this->location.x) {
        return NormalDir::LEFT;
    } else if ((y == this->location.y || y == this->location.y + 1) && x == this->location.x + 1) {
        return NormalDir::RIGHT;
    }
    return NormalDir::LEFT;
};

AABBFace AABB::getFace(NormalDir normDir) {
    switch(normDir) {
        case NormalDir::LEFT:
            return this->wf_left;
            break;
        case NormalDir::RIGHT:
            return this->wf_right;
            break;
        case NormalDir::UP:
            return this->wf_up;
            break;
        case NormalDir::DOWN:
            return this->wf_down;
            break;
        default:
            return this->wf_left;
    }
};

AABBFace AABB::getFace(int x, int y) {
    return getFace(getNormDir(x, y));
};

bool AABB::intersect(const Ray& ray, Coords& intersect_point) {
    double tx1 = (this->location.x - ray.ax) * ray.bx;
    double tx2 = (this->location.x + 64 - ray.ax) * ray.bx;

    double tmin = min(tx1, tx2);
    double tmax = max(tx1, tx2);

    double ty1 = (this->location.y - ray.ay) * ray.by;
    double ty2 = (this->location.y + 64 - ray.ay) * ray.by;

    tmin = max(tmin, min(ty1, ty2));
    tmax = min(tmax, max(ty1, ty2));

    intersect_point = Coords(tmax * ray.bx + ray.ay, tmax * ray.by + ray.ay);
    return tmax >= tmin;
};
}