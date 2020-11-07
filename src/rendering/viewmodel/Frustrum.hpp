#pragma once

#include <limits>
#include <algorithm>

using namespace std;

namespace ViewModel {

class Frustrum {
    public:
        Frustrum();
        Frustrum(double fov_x, double fov_y);

        double getFovX() const;
        double getFovY() const;

        void setFovX(double fov_x_new) noexcept;
        void setFovY(double fov_y_new) noexcept;
    private:
        inline double clampFov(double fov, bool is_x) noexcept;

        double fov_x;
        double fov_y;
};

Frustrum::Frustrum(): Frustrum(1.0, 0.0){};

Frustrum::Frustrum(double fov_x, double fov_y) {
    this->fov_x = clampFov(fov_x, true);
    this->fov_y = clampFov(fov_y, false);
};

void Frustrum::setFovX(double fov_x_new) noexcept {
    this->fov_x = fov_x_new;
};

void Frustrum::setFovY(double fov_y_new) noexcept {
    this->fov_y = fov_y_new;
};

double Frustrum::clampFov(double fov, bool is_x) noexcept {
    return max(
        is_x ? 0.01 : 0.0,
        min(
            fov,
            numeric_limits<double>::max()
        )
    );
};

double Frustrum::getFovX() const {
    return this->fov_x;
};

double Frustrum::getFovY() const {
    return this->fov_y;
};
}