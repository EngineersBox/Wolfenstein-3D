#pragma once

#include <string>

#include "../entity/IEntityBase.hpp"

using namespace std;

struct Sprite : public IEntityBase<double, Coordinates> {
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