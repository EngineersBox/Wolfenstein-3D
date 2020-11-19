#pragma once

#include "../../io/logging/GLDebug.cpp"
#include "../../rendering/Globals.hpp"
#include "../../rendering/viewmodel/Camera.hpp"
#include "../../rendering/viewmodel/Frustrum.hpp"
#include "../../environment/base/entity/IEntityBase.cpp"
#include "../world/World.cpp"

using namespace std;

struct Player : public BaseInterface::IEntityBase<double, Coordinates> {
    Player();
    Player(double x, double y, double fov_x, double fov_y, double angle);

    void handleKeyPress(unsigned char key, int x, int y, World &world);

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

void Player::handleKeyPress(unsigned char key, int x, int y, World &world) {
    if (key == 'w') {
        if (world.getAt((int)(this->location.x + this->camera.frustrum.getFovX() * this->moveSpeed), (int)this->location.y).wf_left.texture == "") {
            this->location.x += this->camera.frustrum.getFovX() * this->moveSpeed;
        }
        if (world.getAt((int)this->location.x, (int)(this->location.y + this->camera.frustrum.getFovY() * this->moveSpeed)).wf_left.texture == "") {
            this->location.y += this->camera.frustrum.getFovY() * this->moveSpeed;
        }
    } else if (key == 's') {
        if (world.getAt((int)(this->location.x - this->camera.frustrum.getFovX() * this->moveSpeed), (int)this->location.y).wf_left.texture == "") {
            this->location.x -= this->camera.frustrum.getFovX() * this->moveSpeed;
        }
        if (world.getAt((int)this->location.x, (int)(this->location.y - this->camera.frustrum.getFovY() * this->moveSpeed)).wf_left.texture == "") {
            this->location.y -= this->camera.frustrum.getFovY() * this->moveSpeed;
        }
    } else if (key == 'a') {
        double old_dir_x = this->camera.frustrum.getFovX();
        this->camera.frustrum.setFovX(this->camera.frustrum.getFovX() * cos(-this->rotSpeed) - this->camera.frustrum.getFovY() * sin(-this->rotSpeed));
        this->camera.frustrum.setFovY(old_dir_x * sin(-this->rotSpeed) + this->camera.frustrum.getFovY() * cos(-this->rotSpeed));

        double old_plane_x = this->camera.clip_plane_x;
        this->camera.clip_plane_x = this->camera.clip_plane_x * cos(-this->rotSpeed) - this->camera.clip_plane_y * sin(-this->rotSpeed);
        this->camera.clip_plane_y = old_plane_x * sin(-this->rotSpeed) + this->camera.clip_plane_y * cos(-this->rotSpeed);
    } else if (key == 'd') {
        double old_dir_x = this->camera.frustrum.getFovX();
        this->camera.frustrum.setFovX(this->camera.frustrum.getFovX() * cos(this->rotSpeed) - this->camera.frustrum.getFovY() * sin(this->rotSpeed));
        this->camera.frustrum.setFovY(old_dir_x * sin(this->rotSpeed) + this->camera.frustrum.getFovY() * cos(this->rotSpeed));

        double old_plane_x = this->camera.clip_plane_x;
        this->camera.clip_plane_x = this->camera.clip_plane_x * cos(this->rotSpeed) - this->camera.clip_plane_y * sin(this->rotSpeed);
        this->camera.clip_plane_y = old_plane_x * sin(this->rotSpeed) + this->camera.clip_plane_y * cos(this->rotSpeed);
    }
}