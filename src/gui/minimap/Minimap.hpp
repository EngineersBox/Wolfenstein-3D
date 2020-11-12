#pragma once

#include "../../rendering/Globals.hpp"
#include "../../rendering/drawing/DrawingUtils.hpp"
#include "../../rendering/colour/Colours.hpp"
#include "../../environment/player/Player.hpp"

using namespace std;

namespace GUI {

class Minimap {
    public:
        Minimap(){};
        Minimap(Player* player, World* map, int screen_width, int screen_height);
        
        void render(int screen_width, int screen_height);

        float getScalingX() const;
        float getScalingY() const;
        float getOffsetX() const;
        float getOffsetY() const;
    private:
        void renderPlayerPos();
        void renderMap2D();

        float scalingX;
        float scalingY;

        int xOffset;
        int yOffset;

        int screenW;
        int screenH;

        Player* player;
        World* world;
};

Minimap::Minimap(Player* player, World* map, int screen_width, int screen_height) {
    this->player = player;
    this->world = map;
    this->screenW = screen_width;
    this->screenH = screen_height;
    this->xOffset = minimapCfg.isLeft() ? 0 : screen_width - minimapCfg.size;
    this->yOffset = minimapCfg.isTop() ? 0 : screen_height - minimapCfg.size;
    this->scalingX = ((float)minimapCfg.size) / ((float)map->map_width);
    this->scalingY = ((float)minimapCfg.size) / ((float)map->map_height);
};

void Minimap::renderPlayerPos() {
    Colour::RGB_Red.toColour4d();
    glPointSize(8);

    // Draw player point
    glBegin(GL_POINTS);
    glVertex2d(xOffset + (player->location.x * scalingY), yOffset + (player->location.y * scalingX));

    glEnd();

    // Draw direction vector
    renderRay(
        xOffset + (player->location.x * scalingX),
        yOffset + (player->location.y * scalingY),
        xOffset + ((player->location.x + player->camera.frustrum.getFovX() * 1.5) * scalingX),
        yOffset + ((player->location.y + player->camera.frustrum.getFovY() * 1.5) * scalingY),
        3,
        Colour::RGB_Red
    );
};

void Minimap::renderMap2D() {
    int x, y;
    for (y = 0; y < world->map_height; y++) {
        for (x = 0; x < world->map_width; x++) {
            // Change to colour coresponding to map location
            if (world->getAt(x,y).wf_left.texture != "") {
                glColor3i(0,0,0);
            } else {
                world->getAt(x, y).wf_left.colour.toColour4d();
            }
            drawRectangle(
                xOffset + x * scalingX,
                yOffset + y * scalingY,
                scalingX,
                scalingY
            );
        }
    }
};

void Minimap::render(int screen_width, int screen_height) {
    this->screenW = screen_width;
    this->screenH = screen_height;
    GLint current_colour[4];
    glGetIntegerv(GL_CURRENT_COLOR, current_colour);

    renderMap2D();
    renderPlayerPos();

    glColor3i(
        current_colour[0],
        current_colour[1],
        current_colour[2]
    );
};

float Minimap::getScalingX() const {
    return this->scalingX;
};

float Minimap::getScalingY() const {
    return this->scalingY;
}

float Minimap::getOffsetX() const {
    return this->xOffset;
};

float Minimap::getOffsetY() const {
    return this->yOffset;
}
}