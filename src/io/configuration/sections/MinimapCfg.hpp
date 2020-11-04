#pragma once

using namespace std;

enum MinimapPos {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

enum MinimapSize: int {
    SMALL = 30,
    MEDIUM = 20,
    LARGE = 10
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

MinimapSize parseMinimapSize(const string& pos_str) {
    if (pos_str == "SMALL") {
        return MinimapSize::SMALL;
    } else if (pos_str == "MEDIUM") {
        return MinimapSize::MEDIUM;
    } else if (pos_str == "LARGE") {
        return MinimapSize::LARGE;
    }
    return MinimapSize::MEDIUM;
}

struct MinimapCfg {
    inline bool isTop();
    inline bool isLeft();

    bool enable;
    bool render_rays;
    MinimapPos pos;
    MinimapSize size;
};

inline bool MinimapCfg::isTop() {
    return this->pos == TOP_LEFT || this->pos == TOP_RIGHT;
};

inline bool MinimapCfg::isLeft() {
    return this->pos == TOP_LEFT || this->pos == BOTTOM_LEFT;
};