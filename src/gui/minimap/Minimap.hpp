#pragma once

#include "../../rendering/Globals.hpp"
#include "../../rendering/drawing/DrawingUtils.hpp"
#include "../../rendering/colour/Colours.hpp"
#include "../../rendering/player/Player.hpp"

using namespace std;

class Minimap {
    public:
        Minimap(){};
        Minimap(Player* player, GameMap* map, int screen_width, int screen_height);
        
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
        GameMap* gameMap;
};

Minimap::Minimap(Player* player, GameMap* map, int screen_width, int screen_height) {
    this->player = player;
    this->gameMap = map;
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
    glVertex2d(xOffset + (player->x * scalingY), yOffset + (player->y * scalingX));

    glEnd();

    // Draw direction vector
    renderRay(
        xOffset + (player->x * scalingX),
        yOffset + (player->y * scalingY),
        xOffset + ((player->x + player->dx * 1.5) * scalingX),
        yOffset + ((player->y + player->dy * 1.5) * scalingY),
        3,
        Colour::RGB_Red
    );
};

void Minimap::renderMap2D() {
    int x, y;
    for (y = 0; y < gameMap->map_height; y++) {
        for (x = 0; x < gameMap->map_width; x++) {
            // Change to colour coresponding to map location
            if (gameMap->getAt(x,y).wf_left.texture != "") {
                glColor3i(0,0,0);
            } else {
                gameMap->getAt(x, y).wf_left.colour.toColour4d();
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