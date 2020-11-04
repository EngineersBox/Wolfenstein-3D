#pragma once

#include "../../io/logging/GLDebug.hpp"
#include "../../rendering/Globals.hpp"
#include "../camera/Camera.hpp"

using namespace std;

struct Player {
    Player();
    Player(double x, double y, double dx, double dy, double angle);
    void printLocation();
    void logLocation();

    double x;
    double y;
    double dx;
    double dy;
    double angle;
    double moveSpeed;
    double rotSpeed;

    Camera camera{0,0,0,0.66};
};

Player::Player(){};

Player::Player(double x, double y, double dx, double dy, double angle) {
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;
    this->angle = angle;
};

void Player::printLocation() {
    cout << "Player Position: (" << to_string(this->x) << "," << to_string(this->y) << ")" << endl;
};

void Player::logLocation() {
    debugContext.logAppInfo(string("Player Position: (" + to_string(this->x) + "," + to_string(this->y) +")"));
};