#pragma once

#include <string>

#include "../texturing/TextureColours.hpp"
#include "../texturing/texture.hpp"

using namespace std;

class AABBFace {
    public:
        AABBFace();
        AABBFace(Colour colour, string tex);
        ~AABBFace();

        Colour f_colour;
        string f_texture;
};

AABBFace::AABBFace() {
    this->f_colour = NONE;
    this->f_texture = "";
}

AABBFace::AABBFace(Colour colour, string tex) {
    this->f_colour = colour;
    this->f_texture = tex;
};

AABBFace::~AABBFace(){};