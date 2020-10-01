#pragma once

#include <string>

#include "../texturing/TextureColours.hpp"
#include "../texturing/texture.hpp"

using namespace std;

class WallFace {
    public:
        WallFace();
        WallFace(Colour colour, string tex);
        ~WallFace();

        Colour f_colour;
        string f_texture;
};

WallFace::WallFace() {
    this->f_colour = NONE;
    this->f_texture = "";
}

WallFace::WallFace(Colour colour, string tex) {
    this->f_colour = colour;
    this->f_texture = tex;
};

WallFace::~WallFace(){};