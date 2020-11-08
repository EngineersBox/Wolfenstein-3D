#pragma once

#include <vector>
#include <string>

#include "Sprite.hpp"

using namespace std;

namespace Constructs {

class Enemy : public Sprite {
    public:
        Enemy(){};
        Enemy(vector<string> animation_frames, Tick animation_tick_speed);
    private:
        vector<string> animation_frames;
        int frame_idx = 0;
        Tick animation_tick_speed;
};

Enemy::Enemy(vector<string> animation_frames, Tick animation_tick_speed) {
    this->animation_frames = animation_frames;
    this->animation_tick_speed = animation_tick_speed;
};

void Enemy::update() {
    this->tick++;
    if (this->tick == this->animation_tick_speed) {
        this->texture = this->animation_frames[this->frame_idx];
        this->frame_idx = (this->frame_idx + 1) % this->animation_frames.size();
        this->tick = 0;
    }
};
}