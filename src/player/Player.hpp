#pragma once

#include "../logging/GLDebug.hpp"
#include "../raytracer/Globals.hpp"

using namespace std;

class Player {
    public:
        Player();
        Player(double x, double y, double dx, double dy, double angle);
        ~Player();
        void printLocation();
        void logLocation();

        double x;
        double y;
        double dx;
        double dy;
        double angle;
};

Player::Player(){};

Player::Player(double x, double y, double dx, double dy, double angle) {
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;
    this->angle = angle;
};

Player::~Player() {};

void Player::printLocation() {
    cout << "Player Position: (" << to_string(this->x) << "," << to_string(this->y) << ")" << endl;
};

void Player::logLocation() {
    debugContext.logAppInfo(string("Player Position: (" + to_string(this->x) + "," + to_string(this->y) +")"));
};