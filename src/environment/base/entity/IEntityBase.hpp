#pragma once

#include "../../../rendering/texturing/texture.hpp"
#include "../../../rendering/Globals.hpp"
#include "../../../physics/Interaction.hpp"
#include "../../../logic/id/IDGenerator.hpp"

using namespace std;

namespace BaseInterface {

template <typename T = int, template <typename> class C = Coordinates>
struct IEntityBase {
    IEntityBase(C<T> eloc, INTERACTION_TYPE itype);
    virtual bool operator==(IEntityBase<T, C>& other);
    int_id ent_id = IDGenerator::instance()->next();
    C<T> location;
    INTERACTION_TYPE interaction_type;
};

template <typename T, template <typename> class C>
bool IEntityBase<T,C>::operator==(IEntityBase<T, C>& other) {
    return (this->location == other.location
        &&  this->interaction_type == other.interaction_type);
};

template <typename T, template <typename> class C>
IEntityBase<T,C>::IEntityBase(C<T> eloc, INTERACTION_TYPE itype):
    location(eloc),
    interaction_type(itype)
{};
}