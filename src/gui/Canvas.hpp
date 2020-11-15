#pragma once

#include <unordered_map>

#include "base/IBaseElement.hpp"
#include "button/Button.hpp"
#include "debug_overlay/DebugOverlay.hpp"
#include "minimap/Minimap.hpp"
#include "stats_bar/StatsBar.hpp"

using namespace std;

namespace GUI {

class Canvas {
    public:
        Canvas(){};

        void addButton(Button* element);
        void removeButton(int_id id);

        void setDebugOverlay(DebugOverlay overlay);
        DebugOverlay& getDebugOverlay();

        void setMinimap(Minimap minimap);
        Minimap& getMinimap();

        void setStatsBar(StatsBar statsbar);
        StatsBar& getStatsBar();

        void handleMouse(int button, int state, int posx, int posy);
        void handleKeypress(unsigned char key, int x, int y);

        void render(double frame_time);
    private:
        unordered_map<int_id, Button*> buttons;
        DebugOverlay overlay;
        Minimap minimap;
        StatsBar statsbar;
};

void Canvas::addButton(Button* element) {
    this->buttons.emplace(make_pair(element->id, element));
};

void Canvas::removeButton(int_id id) {
    this->buttons.erase(id);
};

void Canvas::setDebugOverlay(DebugOverlay overlay) {
    this->overlay = overlay;
};

DebugOverlay& Canvas::getDebugOverlay() {
    return this->overlay;
};

void Canvas::setMinimap(Minimap minimap) {
    this->minimap = minimap;
};

Minimap& Canvas::getMinimap() {
    return this->minimap;
};

void Canvas::setStatsBar(StatsBar statsbar) {
    this->statsbar = statsbar;
};

StatsBar& Canvas::getStatsBar() {
    return this->statsbar;
};

void Canvas::handleMouse(int button, int state, int posx, int posy) {
    for (pair<const int_id, Button*> elem : this->buttons) {
        elem.second->handleMouse(button, state, posx, posy);
    }
};

void Canvas::handleKeypress(unsigned char key, int x, int y){};

void Canvas::render(double frame_time) {
    for(pair<const int_id, Button*> elem : this->buttons) {
        elem.second->render();
    }
    this->minimap.render(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    this->overlay.render(frame_time);
    this->statsbar.render(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
};
};