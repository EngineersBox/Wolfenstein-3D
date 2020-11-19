#pragma once

#include "../../rendering/Globals.hpp"
#include "../../rendering/drawing/RasterText.hpp"
#include "../../environment/player/Player.cpp"
#include "../../environment/world/World.cpp"
#include "../minimap/Minimap.cpp"

using namespace std;

namespace GUI {

#define OVERLAY_TEXT_SPACING 15
#define OVERLAY_TEXT_OFFSET_X 10

typedef void* GLfont;

class DebugOverlay {
    public:
        DebugOverlay(){};
        DebugOverlay(Player* player, Minimap* minimap, World* map, GLfont font);

        void render(double frame_time);
    private:
        void renderFPS(double frame_time);
        void renderPlayerPos();
        void renderTimeTick(double new_frame_time);

        GLfont font;
        Player* player;

        int fpsPosX;
        int fpsPosY;
        int playerPosX;
        int playerPosY;
        int timeTickPosX;
        int timeTickPosY;

        float currentFrameTime = 0.0;
};

DebugOverlay::DebugOverlay(Player* player, Minimap* minimap, World* map, GLfont font) {
    this->font = font;
    this->player = player;
    this->fpsPosX = !minimapCfg.isLeft() ? OVERLAY_TEXT_OFFSET_X : minimap->getOffsetX() + (map->map_width * minimap->getScalingX()) + OVERLAY_TEXT_OFFSET_X;
    this->fpsPosY = OVERLAY_TEXT_SPACING;
    this->playerPosX = this->fpsPosX;
    this->playerPosY = this->fpsPosY + OVERLAY_TEXT_SPACING;
    this->timeTickPosX = this->playerPosX;
    this->timeTickPosY = this->playerPosY + OVERLAY_TEXT_SPACING;
};

void DebugOverlay::renderFPS(double frame_time) {
    if (loggingCfg.show_fps) {
        displayText(
            this->fpsPosX, this->fpsPosY,
            Colour::RGB_Yellow, this->font,
            string("[FPS] ") + to_string(1.0 / frame_time).c_str()
        );
    }
    this->currentFrameTime = frame_time;
};

void DebugOverlay::renderPlayerPos() {
    if (loggingCfg.show_player_pos) {
        displayText(
            this->playerPosX, this->playerPosY,
            Colour::RGB_Yellow, this->font,
            string("[Player Position] X: ")
                + to_string(this->player->location.x).c_str() + " Y: "
                + to_string(this->player->location.y).c_str()
        );
    }
};

void DebugOverlay::renderTimeTick(double new_frame_time) {
    if (loggingCfg.show_time_tick) {
        displayText(
            this->timeTickPosX, this->timeTickPosY,
            Colour::RGB_Yellow, this->font,
            string("[Time Tick] Previous: ")
                + to_string(this->currentFrameTime).c_str() + " Current: "
                + to_string(new_frame_time).c_str()
        );
    }
    this->currentFrameTime = new_frame_time;
};

void DebugOverlay::render(double frame_time) {
    renderFPS(frame_time);
    renderPlayerPos();
    renderTimeTick(frame_time);
};
}