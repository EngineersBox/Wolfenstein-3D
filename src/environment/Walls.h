#pragma once

#include <string>
#include <cmath>

#include "../texturing/TextureColours.h"
#include "../texturing/texture.h"

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
        Wall(double x, double y, Colour colour, string texture);
        Wall(double x, double y, Colour colour);
        Wall(double x, double y, string texture);
        Wall();
        ~Wall();
        bool operator==(Wall& other);
        bool operator!=(Wall& other);
        NormalDir getNormDir(float x, float y, int wall_width, int wall_height);
        double posX;
        double posY;
        Colour texColour;
        string texture_name;
};

///
/// Wall object that stores a location and colour
///
/// @param double x: X-axis location
/// @param double y: Y-axis location
/// @param Colour colour: A colour to set as the wall
/// @param string texture: Name of texture for the wall
///
/// @returns Wall
///
Wall::Wall(double x, double y, Colour colour, string texture) {
    this->posX = x;
    this->posY = y;
    this->texColour = colour;
    this->texture_name = texture;
}

Wall::Wall() : Wall(0, 0, NONE, "") {};

Wall::Wall(double x, double y, string texture) : Wall(x, y, NONE, texture) {};

Wall::Wall(double x, double y, Colour colour) : Wall(x, y, colour, "") {};

Wall::~Wall() {};

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