#pragma once

#include <string>

#include "../texturing/TextureColours.h"
#include "../texturing/texture.h"

class Wall {
    double posX;
    double posY;
    Colour texColour;
    string texture_name;
    Texture texture;

    private:
        Wall(double x, double y, Colour colour, string texture);

    public:
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
        Texture getTexture();
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
    if (texture != "") {
        this->texture = Texture(texture, texture, 64, 64);
    }
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

Texture Wall::getTexture() {
    return texture;
}