#pragma once

#include <string>
#include <vector>

#include "../../rendering/Globals.hpp"
#include "../../rendering/drawing/DrawingUtils.hpp"
#include "../../rendering/colour/Colours.cpp"

using namespace std;

namespace GUI {

#define SB_SECTION_COUNT 8

#define SB_LEVEL_RATIO 0.1
#define SB_SCORE_RATIO 0.2
#define SB_LIVES_RATIO 0.1
#define SB_FACE_RATIO 0.1
#define SB_HEALTH_RATIO 0.15
#define SB_AMMO_RATIO 0.1
#define SB_TOOLS_RATIO 0.05
#define SB_WEAPON_RATIO 0.2

static const float sectionRatios[SB_SECTION_COUNT] = {
    SB_LEVEL_RATIO,
    SB_SCORE_RATIO,
    SB_LIVES_RATIO,
    SB_FACE_RATIO,
    SB_HEALTH_RATIO,
    SB_AMMO_RATIO,
    SB_TOOLS_RATIO,
    SB_WEAPON_RATIO
};

#define BAR_HEIGHT 85

// +-----+-----+-----+----+------+----+-----+------+
// |Level|Score|Lives|Face|Health|Ammo|Tools|Weapon|
// +-----+-----+-----+----+------+----+-----+------+
class StatsBar {
    public:
        StatsBar(){};
        StatsBar(int screen_width, int screen_height, Colour::RGB background_colour, Colour::RGB section_colour, Colour::RGB text_colour);

        void render(int screen_width, int screen_height);
    private:
        inline void renderBarBackground(int screen_width, int screen_height);
        inline void renderBarSections(int screen_width, int screen_height);

        inline void renderText();
        inline void renderWeapon();
        inline void renderFace();

        float posX;
        float posY;

        float innerXOffset;
        float innerYOffset;
        float innerWidth;
        float innerHeight;

        int level;
        int score;
        int lives;
        int faceIdx;
        int health;
        int ammo;
        vector<int> tools;
        string weapon;

        Colour::RGB background_colour;
        Colour::RGB section_colour;
        Colour::RGB text_colour;
};

StatsBar::StatsBar(int screen_width, int screen_height, Colour::RGB background_colour, Colour::RGB section_colour, Colour::RGB text_colour) {
    this->posY = screen_height - BAR_HEIGHT;
    this->posX = 0;
    this->background_colour = background_colour;
    this->section_colour = section_colour;
    this->text_colour = text_colour;
    this->innerXOffset = 5;
    this->innerYOffset = 5;
    this->innerWidth = screen_width - (2 * this->innerXOffset);
    this->innerHeight = BAR_HEIGHT - (2 * this->innerYOffset);
};

inline void StatsBar::renderBarBackground(int screen_width, int screen_height) {
    this->background_colour.toColour4d();
    drawRectangle(this->posX, this->posY, screen_width, screen_height);
};

inline void StatsBar::renderBarSections(int screen_width, int screen_height) {
    float runningOffset = this->innerXOffset;
    float innerBarOffsetY = screen_height - BAR_HEIGHT + this->innerYOffset;
    for (int i = 0; i < SB_SECTION_COUNT; i++) {
        this->section_colour.toColour4d();
        drawRectangle(
            runningOffset,
            innerBarOffsetY,
            this->innerWidth * sectionRatios[i],
            this->innerHeight
        );
        drawOutline(
            runningOffset,
            innerBarOffsetY,
            runningOffset + (this->innerWidth * sectionRatios[i]),
            screen_height - this->innerYOffset,
            3,
            Colour::RGB_Grey
        );
        runningOffset += this->innerWidth * sectionRatios[i];
    }
};

void StatsBar::render(int screen_width, int screen_height) {
    if (!renderCfg.show_stats_bar) {
        return;
    }
    GLint current_colour[4];
    glGetIntegerv(GL_CURRENT_COLOR, current_colour);

    renderBarBackground(screen_width, screen_height);
    renderBarSections(screen_width, screen_height);

    glColor3i(
        current_colour[0],
        current_colour[1],
        current_colour[2]
    );
}
}