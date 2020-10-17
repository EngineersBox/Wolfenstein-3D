#pragma once

class Ray {
    public:
        Ray();
        Ray(float ax, float ay, float bx, float by, int line_width);
        ~Ray();

        float ax;
        float ay;
        float bx;
        float by;

        int signX;
        int signY;

        int line_width;
};

Ray::~Ray(){};

Ray::Ray():
    Ray(0.0,0.0,0.0,0.0,0)
{};

Ray::Ray(float ax, float ay, float bx, float by, int line_width) {
    this->ax = ax;
    this->ay = ay;
    this->bx = bx;
    this->by = by;
    this->signX = (bx - ax < 0);
    this->signY = (by - ay < 0);
    this->line_width = line_width;
};