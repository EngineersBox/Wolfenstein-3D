#pragma once

#include <GLUT/Glut.h>
#include <tuple>

template <class R, class G, class B, class A>
using Tuple = std::tuple<R, G, B, A>;
using Colour = Tuple<GLdouble, GLdouble, GLdouble, GLdouble>;

Colour RED = {1.0, 0.0, 0.0, 1.0};
Colour YELLOW = {1.0, 1.0, 0.0, 1.0};
Colour GREEN = {0.0, 1.0, 0.0, 1.0};
Colour BLUE = {0.0, 0.0, 1.0, 1.0};
Colour BLACK = {0.0, 0.0, 0.0, 1.0};
Colour WHITE = {1.0, 1.0, 1.0, 1.0};
Colour NONE = {-1.0, -1.0, -1.0, 1.0};

constexpr int RED_IDX = 0;
constexpr int GREEN_IDX = 1;
constexpr int BLUE_IDX = 2;
constexpr int ALPHA_IDX = 3;

///
/// Render colour with OpenGL glColor3d
///
/// @param ColourTuple texColour: Tuple of <R,G,B> values for colour
///
/// @return void
///
void toColour(Colour texColour)
{
    glColor3d(
        std::get<RED_IDX>(texColour),
        std::get<GREEN_IDX>(texColour),
        std::get<BLUE_IDX>(texColour));
}