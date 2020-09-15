#pragma once

#include <GLUT/Glut.h>
#include <tuple>
#include <string>

using namespace std;
template <class R, class G, class B, class A>
using Tuple = tuple<R, G, B, A>;
using Colour = Tuple<GLdouble, GLdouble, GLdouble, GLdouble>;


Colour RED = {1.0, 0.0, 0.0, 1.0};
Colour YELLOW = {1.0, 1.0, 0.0, 1.0};
Colour GREEN = {0.0, 1.0, 0.0, 1.0};
Colour BLUE = {0.0, 0.0, 1.0, 1.0};
Colour BLACK = {0.0, 0.0, 0.0, 1.0};
Colour WHITE = {1.0, 1.0, 1.0, 1.0};
Colour NONE = {-1.0, -1.0, -1.0, 1.0};
const int RED_IDX = 0;
const int GREEN_IDX = 1;
const int BLUE_IDX = 2;
const int ALPHA_IDX = 3;

///
/// Render colour with OpenGL glColor3d
///
/// @param ColourTuple texColour: Tuple of <R,G,B> values for colour
///
/// @return void
///
void toColour(Colour texColour) {
    glColor3d(
        get<RED_IDX>(texColour),
        get<GREEN_IDX>(texColour),
        get<BLUE_IDX>(texColour));
}

Colour toTexColour(string colour) {
    if (colour == "RED") {
        return RED;
    } else if (colour == "YELLOW") {
        return YELLOW;
    } else if (colour == "GREEN") {
        return GREEN;
    } else if (colour == "BLUE") {
        return BLUE;
    } else if (colour == "BLACK") {
        return BLACK;
    } else if (colour == "WHITE") {
        return WHITE;
    } else {
        return NONE;
    }
}

void printColour(Colour colour) {
    if (sizeof colour == 0) {
        cout << "EMPTY" << endl;
        return;
    }
    cout << "R:" << get<RED_IDX>(colour);
    cout << " G:" << get<GREEN_IDX>(colour);
    cout << " B:" << get<BLUE_IDX>(colour);
    cout << " A:" << get<ALPHA_IDX>(colour) << endl;
}