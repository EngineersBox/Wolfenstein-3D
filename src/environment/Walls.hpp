#pragma once

#include <cmath>
#include <string>

#include "../texturing/TextureColours.hpp"
#include "../texturing/texture.hpp"
#include "WallFace.hpp"

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
    NormalDir getNormDir(int x, int y);
    WallFace getFace(NormalDir normDir);
    WallFace getFace(int x, int y);

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
};

bool Wall::operator!=(Wall& other) {
    return (this->posX != other.posX)
        && (this->posY != other.posY)
        && (this->texColour != other.texColour)
        && (this->texture_name != other.texture_name);
};

NormalDir Wall::getNormDir(int x, int y) {
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

WallFace Wall::getFace(NormalDir normDir) {
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

WallFace Wall::getFace(int x, int y) {
    return getFace(getNormDir(x, y));
};