#pragma once

#include <string>

#include "../../../rendering/colour/Colours.hpp"
#include "../../../rendering/texturing/texture.hpp"

using namespace std;

class AABBFace {
    public:
        AABBFace();
        AABBFace(Colour::ColorRGB colour, string tex);
        ~AABBFace();

        Colour::ColorRGB f_colour;
        string f_texture;
};

AABBFace::AABBFace() {
    this->f_colour = Colour::RGB_None;
    this->f_texture = "";
}

AABBFace::AABBFace(Colour::ColorRGB colour, string tex) {
    this->f_colour = colour;
    this->f_texture = tex;
};

AABBFace::~AABBFace(){};