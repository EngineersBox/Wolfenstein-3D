#pragma once

#include "../../io/logging/GLDebug.hpp"
#include "../../rendering/Globals.hpp"
#include "../../rendering/viewmodel/Camera.hpp"
#include "../../rendering/viewmodel/Frustrum.hpp"
#include "../../environment/base/entity/IEntityBase.hpp"

using namespace std;

struct Player : public BaseInterface::IEntityBase<double, Coordinates> {
    Player();
    Player(double x, double y, double fov_x, double fov_y, double angle);
    void printLocation();
    void logLocation();

    double angle;
    double moveSpeed;
    double rotSpeed;

    bool isShooting = false;

    ViewModel::Camera camera{0,0,0,0.66, ViewModel::Frustrum()};
};

Player::Player():
    IEntityBase<double, Coordinates>(
        Coordinates<double>(0, 0),
        INTERACTION_TYPE::BLOCK_ALL
    )
{};

Player::Player(double x, double y, double fov_x, double fov_y, double angle):
    IEntityBase<double, Coordinates>(
        Coordinates<double>(x, y),
        INTERACTION_TYPE::BLOCK_ALL
    )
{
    this->camera.frustrum.setFovX(fov_x);
    this->camera.frustrum.setFovY(fov_y);
    this->angle = angle;
};

void Player::printLocation() {
    cout << "Player Position: (" << to_string(this->location.x) << "," << to_string(this->location.y) << ")" << endl;
};

void Player::logLocation() {
    debugContext.logAppInfo(string("Player Position: (" + to_string(this->location.x) + "," + to_string(this->location.y) +")"));
};