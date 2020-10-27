#pragma once

#include "../raytracer/Globals.hpp"
#include "../physics/Interaction.hpp"
#include "../id/IDGenerator.hpp"

using namespace std;

template <typename T = int, template <typename> class C = Coordinates>
struct IObjectBase {
    virtual ~IObjectBase(){};
    virtual void render() = 0;
    virtual bool operator==(IObjectBase<T, C>& other);
    int_id obj_id = IDGenerator::instance()->next();
    C<T> location;
    INTERACTION_TYPE interaction_type;
};

template <typename T, template <typename> class C>
bool IObjectBase<T,C>::operator==(IObjectBase<T, C>& other) {
    return (this->location == other.location
        &&  this->interaction_type == other.interaction_type);
};