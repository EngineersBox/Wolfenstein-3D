#pragma once

#include <string>

#include "../../../rendering/colour/Colours.cpp"
#include "../../../rendering/texturing/texture.cpp"

using namespace std;

namespace Constructs {

class AABBFace {
    public:
        AABBFace();
        AABBFace(Colour::RGB colour, string tex);
        ~AABBFace();

        Colour::RGB colour;
        string texture;
};

AABBFace::AABBFace() {
    this->colour = Colour::RGB_None;
    this->texture = "";
}

AABBFace::AABBFace(Colour::RGB colour, string tex) {
    this->colour = colour;
    this->texture = tex;
};

AABBFace::~AABBFace(){};
}