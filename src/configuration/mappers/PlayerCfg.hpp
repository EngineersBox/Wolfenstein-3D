#pragma once

using namespace std;

class PlayerCfg {
    public:
        PlayerCfg();
        PlayerCfg(float fov, float dof, float move_speed, float rotation_speed);

        float fov;
        float dof;
        float move_speed;
        float rotation_speed;
};

PlayerCfg::PlayerCfg() {};

PlayerCfg::PlayerCfg(float fov, float dof, float move_speed, float rotation_speed) {
    this->fov = fov;
    this->dof = dof;
    this->move_speed = move_speed;
    this->rotation_speed = rotation_speed;
};