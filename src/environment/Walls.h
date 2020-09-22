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
    double posX;
    double posY;
    Colour texColour;
    string texture_name;
    public:
        Wall(double x, double y, Colour colour, string texture);
        Wall(double x, double y, Colour colour);
        Wall(double x, double y, string texture);
        Wall();
        ~Wall();
        double getX();
        void setX(double);
        double getY();
        void setY(double);
        Colour getColour();
        void setColour(Colour);
        string getTextureId();
        NormalDir getNormDir(float x, float y, int wall_width, int wall_height);
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
    posX = x;
    posY = y;
    texColour = colour;
    texture_name = texture;
    // cout << "TEX LOADED: " << texture << endl;
}

Wall::Wall() : Wall(0, 0, NONE, "") {}

Wall::Wall(double x, double y, string texture) : Wall(x, y, NONE, texture) {}

Wall::Wall(double x, double y, Colour colour) : Wall(x, y, colour, "") {}

Wall::~Wall() {}

///
/// Get the x value
///
/// @return double
///
double Wall::getX() {
    return posX;
}

///
/// Set the x value
///
/// @param double newX: Set the X-axis location
///
/// @return void
///
void Wall::setX(double newX) {
    posX = newX;
}

///
/// Get the y value
///
/// @return double
///
double Wall::getY() {
    return posY;
}

///
/// Set the y value
///
/// @param double newY: Set the Y-axis location
///
/// @return void
///
void Wall::setY(double newY) {
    posY = newY;
}

///
/// Get the colour value
///
/// @return Colour
///
Colour Wall::getColour() {
    return texColour;
}

///
/// Set the colour value
///
/// @param Colour newColour: Set the colour for the wall
///
/// @return void
///
void Wall::setColour(Colour newColour) {
    texColour = newColour;
}

string Wall::getTextureId() {
    return texture_name;
}

NormalDir Wall::getNormDir(float x, float y, int wall_width, int wall_height) {
    float centreX = (posX * wall_width) + (wall_width << 1);
    float centreY = (posY * wall_height) + (wall_height << 1);

    float dx = x - centreX;
    float dy = y - centreY;

    float angle = 1 / (tan(dy/dx));
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
}