#pragma once

#include "Frustrum.hpp"

using namespace std;

namespace ViewModel {

struct Camera {
    double x;
    double y;
    double clip_plane_x;
    double clip_plane_y;
    Frustrum frustrum;
};

}