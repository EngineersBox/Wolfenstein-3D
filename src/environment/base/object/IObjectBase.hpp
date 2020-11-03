#pragma once

#include "../../../rendering/Globals.hpp"
#include "../../../physics/Interaction.hpp"
#include "../../../logic/id/IDGenerator.hpp"

using namespace std;

template <typename T = int, template <typename> class C = Coordinates>
struct IObjectBase {
    IObjectBase(C<T> eloc, INTERACTION_TYPE itype);
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

template <typename T, template <typename> class C>
IObjectBase<T,C>::IObjectBase(C<T> eloc, INTERACTION_TYPE itype):
    location(eloc),
    interaction_type(itype)
{};