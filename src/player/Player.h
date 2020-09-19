#pragma once

using namespace std;

class Player {
    public:
        Player();
        Player(float x, float y, float dx, float dy, float angle);
        ~Player();

        float x;
        float y;
        float dx;
        float dy;
        float angle;
};

Player::Player(){};

Player::Player(float x, float y, float dx, float dy, float angle) {
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;
    this->angle = angle;
};

Player::~Player() {};