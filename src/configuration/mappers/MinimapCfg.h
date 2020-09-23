#pragma once

using namespace std;

enum MinimapPos {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

MinimapPos parseMinimapPos(const string& pos_str) {
    if (pos_str == "TOP_LEFT") {
        return MinimapPos::TOP_LEFT;
    } else if (pos_str == "TOP_RIGHT") {
        return MinimapPos::TOP_RIGHT;
    } else if (pos_str == "BOTTOM_LEFT") {
        return MinimapPos::BOTTOM_LEFT;
    } else if (pos_str == "BOTTOM_RIGHT") {
        return MinimapPos::BOTTOM_RIGHT;
    }
    return MinimapPos::TOP_LEFT;
}

class MinimapCfg {
    public:
        MinimapCfg();
        MinimapCfg(bool enable, bool render_rays, MinimapPos pos);
        ~MinimapCfg();

        inline bool isTop();
        inline bool isLeft();

        bool enable;
        bool render_rays;
        MinimapPos pos;
};

MinimapCfg::MinimapCfg() {};

MinimapCfg::MinimapCfg(bool enable, bool render_rays, MinimapPos pos) {
    this->enable = enable;
    this->render_rays = render_rays;
    this->pos = pos;
};

MinimapCfg::~MinimapCfg() {};

inline bool MinimapCfg::isTop() {
    return this->pos == TOP_LEFT || this->pos == TOP_RIGHT;
};

inline bool MinimapCfg::isLeft() {
    return this->pos == TOP_LEFT || this->pos == BOTTOM_LEFT;
};