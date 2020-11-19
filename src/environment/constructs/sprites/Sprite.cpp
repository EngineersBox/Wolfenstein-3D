#pragma once

#include <string>

#include "../../base/entity/IEntityBase.cpp"

using namespace std;

namespace Constructs {

struct Sprite : public BaseInterface::IEntityBase<double, Coordinates> {
    Sprite();
    Sprite(double xloc, double yloc, string tex);

    bool operator==(Sprite& other);
    bool operator<(const Sprite& other) const;

    string texture;
    double distance;
    int order;
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

bool Sprite::operator<(const Sprite& other) const {
    // This is deliberately reversed in order to have faster sorting of sprites,
    // Taking half as much time than if it were implemented as you would expect
    return (this->distance > other.distance
        &&  this->order > other.order);
}
}