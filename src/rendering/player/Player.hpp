#pragma once

#include "../../io/logging/GLDebug.hpp"
#include "../../rendering/Globals.hpp"
#include "../viewmodel/Camera.hpp"
#include "../viewmodel/Frustrum.hpp"

using namespace std;

struct Player {
    Player();
    Player(double x, double y, double fov_x, double fov_y, double angle);
    void printLocation();
    void logLocation();

    double x;
    double y;
    double dx;
    double dy;
    double angle;
    double moveSpeed;
    double rotSpeed;

    ViewModel::Camera camera{0,0,0,0.66, ViewModel::Frustrum()};
};

Player::Player(){};

Player::Player(double x, double y, double fov_x, double fov_y, double angle) {
    this->x = x;
    this->y = y;
    this->camera.frustrum.setFovX(fov_x);
    this->camera.frustrum.setFovY(fov_y);
    this->dx = fov_x;
    this->dy = fov_y;
    this->angle = angle;
};

void Player::printLocation() {
    cout << "Player Position: (" << to_string(this->x) << "," << to_string(this->y) << ")" << endl;
};

void Player::logLocation() {
    debugContext.logAppInfo(string("Player Position: (" + to_string(this->x) + "," + to_string(this->y) +")"));
};