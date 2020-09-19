#pragma once

using namespace std;

class PlayerCfg {
    public:
        PlayerCfg();
        PlayerCfg(float fov, float dof, float sensitivity);
        ~PlayerCfg();

        float fov;
        float dof;
        float sensitivity;
};

PlayerCfg::PlayerCfg() {};

PlayerCfg::PlayerCfg(float fov, float dof, float sensitivity) {
    this->fov = fov;
    this->dof = dof;
    this->sensitivity = sensitivity;
};

PlayerCfg::~PlayerCfg() {};