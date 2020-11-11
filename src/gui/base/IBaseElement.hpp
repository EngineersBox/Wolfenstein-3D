#pragma

#include "../../logic/id/IDGenerator.hpp"
#include "../../rendering/buffering/PBO.hpp"
#include "../../rendering/colour/Colours.hpp"

using namespace std;

namespace GUI {

struct IBaseElement {
    IBaseElement(){};
    IBaseElement(int x, int y, int width, int height, Colour::ColorRGB background_colour);
    virtual void render(Rendering::PBO &pbo);
    virtual void render();

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