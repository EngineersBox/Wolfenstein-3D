#pragma once

#include "../src/texturing/texture.hpp"
#include "../src/raytracer/Globals.hpp"
#include "../src/physics/Interaction.hpp"

using namespace std;

template <typename T, template <typename> typename C = Coordinates>
class IEntityBase {
    public:
        virtual void render() = NULL;
        inline C<T> getLocation() const {
            return this->location;
        }
        inline Texture getSprite() const {
            return this->sprite;
        }
        inline INTERACTION_TYPE getInteractionType() const {
            return this->interaction_type;
        }
    private:
        size_t id;
        C<T> location;
        Texture sprite;
        INTERACTION_TYPE interaction_type;
};