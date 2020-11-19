#pragma once

#include "../../logic/id/IDGenerator.cpp"
#include "../../rendering/buffering/PBO.cpp"
#include "../../rendering/colour/Colours.cpp"

using namespace std;

namespace GUI {

struct IBaseElement {
    IBaseElement(int x, int y, int width, int height, Colour::ColorRGB background_colour);
    virtual void render(Rendering::PBO &pbo) = 0;
    virtual void render() = 0;

    int x;
    int y;
    int width;
    int height;

    Colour::ColorRGB background_colour;
    int_id id;
};

IBaseElement::IBaseElement(int x, int y, int width, int height, Colour::ColorRGB background_colour) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->background_colour = background_colour;
    this->id = IDGenerator::instance()->next();
};
}