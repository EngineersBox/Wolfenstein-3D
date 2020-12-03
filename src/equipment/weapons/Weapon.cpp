#pragma once

#include <vector>
#include <string>
#include <map>

#include "../../animation/Sequence.cpp"
#include "../../rendering/buffering/PBO.cpp"
#include "../../rendering/texturing/texture.cpp"
#include "../../rendering/colour/Colours.cpp"

using namespace std;

namespace Equipment {

class Weapon {
    public:
        Weapon(){};
        Weapon(Animation::Sequence sequence, int posx, int posy);

        void render(Rendering::PBO &pbo, map<string, Texture> &textures);
    private:
        Animation::Sequence sequence;
        int posx;
        int posy;
};

Weapon::Weapon(Animation::Sequence sequence, int posx, int posy) {
    this->sequence = sequence;
    this->posx = posx;
    this->posy = posy;
};

void Weapon::render(Rendering::PBO &pbo, map<string, Texture> &textures) {
    string frame = this->sequence.nextFrame();
    Texture texture = textures[frame];
    for (int x = this->posx; x < this->posx + texture.width; x++) {
        for (int y = this->posy; y < this->posy + texture.height; y++) {
            if (x < 0 || x >= pbo.width || y < 0 || y >= pbo.height) {
                continue;
            }
            Colour::RGB colour = Colour::INTtoRGB(
                texture.texture[(y * texture.height) + x]
            );
            if (colour == Colour::RGB_Black) {
                continue;
            }
            pbo.pushToBuffer(x, y, colour);
        }
    }
    this->sequence.update();
};

}