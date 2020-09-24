#pragma once

#include <string>
#include <cmath>

#include "../texturing/TextureColours.h"
#include "../texturing/texture.h"
#include "WallFace.h"

using namespace std;

#define IS_LEFT(a) ((7 * M_PI) / 4) < a&& a <= (M_PI / 4)
#define IS_DOWN(a) (M_PI / 4) < a&& a <= ((3 * M_PI) / 4)
#define IS_RIGHT(a) ((3 * M_PI) / 4) < a&& a <= ((5 * M_PI) / 4)
#define IS_UP(a) ((5 * M_PI) / 4) < a&& a <= ((7 * M_PI) / 4)

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

class Wall {
   public:
    Wall(int x, int y, Colour colour, string texture);
    Wall(int x, int y, Colour colour);
    Wall(int x, int y, string texture);
    Wall(int x, int y, Colour colour, WallFace wf_left, WallFace wf_right, WallFace wf_up, WallFace wf_down);
    Wall();
    ~Wall();
    bool operator==(Wall& other);
    bool operator!=(Wall& other);
    NormalDir getNormDir(float x, float y, int wall_width, int wall_height);
    WallFace getFace(NormalDir normDir);
    WallFace getFace(float x, float y, int wall_width, int wall_height);

    int posX;
    int posY;
    Colour texColour;
    string texture_name;

    WallFace wf_left;
    WallFace wf_right;
    WallFace wf_up;
    WallFace wf_down;
};

///
/// Wall object that stores a location and colour
///
/// @param int x: X-axis location
/// @param int y: Y-axis location
/// @param Colour colour: A colour to set as the wall
/// @param string texture: Name of texture for the wall
///
/// @returns Wall
///
Wall::Wall(int x, int y, Colour colour, string texture) {
    this->posX = x;
    this->posY = y;
    this->texColour = colour;
    this->texture_name = texture;
}

Wall::Wall(int x, int y, Colour colour, WallFace wf_left, WallFace wf_right, WallFace wf_up, WallFace wf_down) {
    this->posX = x;
    this->posY = y;
    this->texColour = colour;
    this->wf_left = wf_left;
    this->wf_right = wf_right;
    this->wf_up = wf_up;
    this->wf_down = wf_down;
};

Wall::Wall() : Wall(0, 0, NONE, "") {};

Wall::Wall(int x, int y, string texture) : Wall(x, y, NONE, texture) {};

Wall::Wall(int x, int y, Colour colour) : Wall(x, y, colour, "") {};

Wall::~Wall(){};

bool Wall::operator==(Wall& other) {
    return (this->posX == other.posX)
        && (this->posY == other.posY)
        && (this->texColour == other.texColour)
        && (this->texture_name == other.texture_name);
}

bool Wall::operator!=(Wall& other) {
    return (this->posX != other.posX)
        && (this->posY != other.posY)
        && (this->texColour != other.texColour)
        && (this->texture_name != other.texture_name);
}

NormalDir Wall::getNormDir(float x, float y, int wall_width, int wall_height) {
    const float centreX = (posX * wall_width) + (wall_width << 1);
    const float centreY = (posY * wall_height) + (wall_height << 1);

    const float dx = x - centreX;
    const float dy = y - centreY;

    const float angle = 1 / (tan(dy/dx));
    if (IS_LEFT(angle)) {
        return NormalDir::LEFT;
    } else if (IS_RIGHT(angle)) {
        return NormalDir::RIGHT;
    } else if (IS_DOWN(angle)) {
        return NormalDir::DOWN;
    } else if (IS_UP(angle)) {
        return NormalDir::UP;
    } else {
        return NormalDir::LEFT;
    }
};

WallFace Wall::getFace(NormalDir normDir) {
    switch(normDir) {
        case NormalDir::LEFT:
            // cout << this->wf_left.f_texture << endl;
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

WallFace Wall::getFace(float x, float y, int wall_width, int wall_height) {
    return getFace(getNormDir(x, y, wall_width, wall_height));
};