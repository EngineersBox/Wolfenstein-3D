#pragma once

#include <cmath>
#include <string>

#include "../texturing/TextureColours.hpp"
#include "../texturing/texture.hpp"
#include "AABBFace.hpp"
#include "../raytracer/Ray.hpp"

using namespace std;

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

class AABB {
   public:
    AABB(int x, int y, Colour colour, string texture);
    AABB(int x, int y, Colour colour);
    AABB(int x, int y, string texture);
    AABB(int x, int y, Colour colour, AABBFace wf_left, AABBFace wf_right, AABBFace wf_up, AABBFace wf_down);
    AABB();

    bool operator==(AABB& other);
    bool operator!=(AABB& other);
    NormalDir getNormDir(int x, int y);
    AABBFace getFace(NormalDir normDir);
    AABBFace getFace(int x, int y);
    bool intersect(const Ray& ray, Coords& intersect_point);

    int posX;
    int posY;
    Colour texColour;
    string texture_name;

    AABBFace wf_left;
    AABBFace wf_right;
    AABBFace wf_up;
    AABBFace wf_down;
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
AABB::AABB(int x, int y, Colour colour, string texture) {
    this->posX = x;
    this->posY = y;
    this->texColour = colour;
    this->texture_name = texture;
}

AABB::AABB(int x, int y, Colour colour, AABBFace wf_left, AABBFace wf_right, AABBFace wf_up, AABBFace wf_down) {
    this->posX = x;
    this->posY = y;
    this->texColour = colour;
    this->wf_left = wf_left;
    this->wf_right = wf_right;
    this->wf_up = wf_up;
    this->wf_down = wf_down;
};

AABB::AABB() : AABB(0, 0, NONE, "") {};

AABB::AABB(int x, int y, string texture) : AABB(x, y, NONE, texture) {};

AABB::AABB(int x, int y, Colour colour) : AABB(x, y, colour, "") {};

bool AABB::operator==(AABB& other) {
    return (this->posX == other.posX)
        && (this->posY == other.posY)
        && (this->texColour == other.texColour)
        && (this->texture_name == other.texture_name);
};

bool AABB::operator!=(AABB& other) {
    return (this->posX != other.posX)
        && (this->posY != other.posY)
        && (this->texColour != other.texColour)
        && (this->texture_name != other.texture_name);
};

NormalDir AABB::getNormDir(int x, int y) {
    // cout << to_string(x) << " " << to_string(y) << endl;
    // cout << to_string(posX) << " " << to_string(posY) << endl;
    if ((x == posX || x == posX + 1) && y == posY) {
        return NormalDir::DOWN;
    } else if ((x == posX || x == posX + 1) && y == posY + 1) {
        return NormalDir::UP;
    } else if ((y == posY || y == posY + 1) && x == posX) {
        return NormalDir::LEFT;
    } else if ((y == posY || y == posY + 1) && x == posX + 1) {
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
    double tx1 = (posX - ray.ax) * ray.bx;
    double tx2 = (posX + 64 - ray.ax) * ray.bx;

    double tmin = min(tx1, tx2);
    double tmax = max(tx1, tx2);

    double ty1 = (posY - ray.ay) * ray.by;
    double ty2 = (posY + 64 - ray.ay) * ray.by;

    tmin = max(tmin, min(ty1, ty2));
    tmax = min(tmax, max(ty1, ty2));

    intersect_point = Coords(tmax * ray.bx + ray.ay, tmax * ray.by + ray.ay);
    return tmax >= tmin;
};