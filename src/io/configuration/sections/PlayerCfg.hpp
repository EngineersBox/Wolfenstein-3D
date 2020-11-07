#pragma once

#include <algorithm>

using namespace std;

namespace ConfigSection {

#define BASE_FOV_VAL 70.0
#define MIN_FOV_VAL 0.01
#define MAX_FOV_VAL 360.0

double scaleFov(double fov) {
    return BASE_FOV_VAL / max(MIN_FOV_VAL, min(fov, MAX_FOV_VAL));
}

struct PlayerCfg {
    PlayerCfg(){};
    PlayerCfg(double fov, float dof, float move_speed, float rotation_speed);
    double fov;
    float dof;
    float move_speed;
    float rotation_speed;
};

PlayerCfg::PlayerCfg(double fov, float dof, float move_speed, float rotation_speed) {
    this->fov = scaleFov(fov);
    cout << this->fov << endl;
    this->dof = dof;
    this->move_speed = move_speed;
    this->rotation_speed = rotation_speed;
}
}