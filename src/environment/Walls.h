#pragma once

#include "../texturing/TextureColours.h"

class Wall
{

    double posX;
    double posY;
    Colour texColour;

public:
    Wall(double, double, Colour);
    double getX();
    void setX(double);
    double getY();
    void setY(double);
    Colour getColour();
    void setColour(Colour);
};

///
/// Wall object that stores a location and colour
///
/// @param double x: X-axis location
/// @param double y: Y-axis location
/// @param Colour colour: A colour to set as the wall
///
/// @returns Wall
///
Wall::Wall(double x, double y, Colour colour)
{
    posX = x;
    posY = y;
    texColour = colour;
}

///
/// Get the x value
///
/// @return double
///
double Wall::getX()
{
    return posX;
}

///
/// Set the x value
///
/// @param double newX: Set the X-axis location
///
/// @return void
///
void Wall::setX(double newX)
{
    posX = newX;
}

///
/// Get the y value
///
/// @return double
///
double Wall::getY()
{
    return posY;
}

///
/// Set the y value
///
/// @param double newY: Set the Y-axis location
///
/// @return void
///
void Wall::setY(double newY)
{
    posY = newY;
}

///
/// Get the colour value
///
/// @return Colour
///
Colour Wall::getColour()
{
    return texColour;
}

///
/// Set the colour value
///
/// @param Colour newColour: Set the colour for the wall
///
/// @return void
///
void Wall::setColour(Colour newColour)
{
    texColour = newColour;
}