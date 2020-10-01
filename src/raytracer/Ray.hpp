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
    this->line_width = line_width;
};