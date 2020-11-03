#pragma once

#include <string>

#include "../../base/entity/IEntityBase.hpp"

using namespace std;

namespace Constructs {

struct Sprite : public BaseInterface::IEntityBase<double, Coordinates> {
    Sprite();
    Sprite(double xloc, double yloc, string tex);
    bool operator==(Sprite& other);
    string texture;
};

Sprite::Sprite():
    IEntityBase<double, Coordinates>(
        Coordinates<double>(0, 0),
        INTERACTION_TYPE::PLAYER_ONLY
    ),
    texture("")
{};

Sprite::Sprite(double xloc, double yloc, string tex):
    IEntityBase<double, Coordinates>(
        Coordinates<double>(xloc, yloc),
        INTERACTION_TYPE::PLAYER_ONLY
    ),
    texture(tex)
{};

bool Sprite::operator==(Sprite& other) {
    return (this->location == other.location
        &&  this->texture == other.texture
        &&  this->interaction_type == other.interaction_type);
}
}