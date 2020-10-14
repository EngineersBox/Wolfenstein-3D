#pragma once

#include "../src/raytracer/Globals.hpp"
#include "../src/physics/Interaction.hpp"

using namespace std;

template <typename T, template <typename> typename C = Coordinates>
class IObjectBase {
    public:
        virtual void render() = NULL;
        inline C<T> getLocation() const {
            return this->location;
        }
        inline INTERACTION_TYPE getInteractionType() const {
            return this->interaction_type;
        }
    private:
        size_t id;
        C<T> location;
        INTERACTION_TYPE interaction_type;
};