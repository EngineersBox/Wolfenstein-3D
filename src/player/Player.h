#pragma once

using namespace std;

class Player {
    public:
        Player();
        Player(float x, float y, float dx, float dy, float angle);
        ~Player();
        void printLocation();

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

void Player::printLocation() {
    cout << "Player Position: (" << to_string(this->x) << "," << to_string(this->y) << ")" << endl;
}