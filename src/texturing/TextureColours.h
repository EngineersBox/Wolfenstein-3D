#pragma once

#include <GLUT/Glut.h>
#include <tuple>
#include <string>
#include <vector>

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
Colour DARK_GREY = {0.3, 0.3, 0.3, 1.0};
Colour LIGHT_GREY = {0.7, 0.7, 0.7, 1.0};
Colour NONE = {-1.0, -1.0, -1.0, 1.0};

const int RED_IDX = 0;
const int GREEN_IDX = 1;
const int BLUE_IDX = 2;
const int ALPHA_IDX = 3;

#define GET_RED(c) get<RED_IDX>(c)
#define GET_GREEN(c) get<GREEN_IDX>(c)
#define GET_BLUE(c) get<BLUE_IDX>(c)
#define GET_ALPHA(c) get<ALPHA_IDX>(c)

///
/// Render colour with OpenGL glColor3d
///
/// @param ColourTuple texColour: Tuple of <R,G,B> values for colour
///
/// @return void
///
void toColour(Colour texColour) {
    glColor4d(
        GET_RED(texColour),
        GET_GREEN(texColour),
        GET_BLUE(texColour),
        GET_ALPHA(texColour)
    );
}

void toClearColour(Colour texColour) {
    glClearColor(
        GET_RED(texColour) / 255,
        GET_GREEN(texColour) / 255,
        GET_BLUE(texColour) / 255,
        GET_ALPHA(texColour) / 255
    );
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
        cout << "EMPTY COLOUR" << endl;
        return;
    }
    cout << "R:" << get<RED_IDX>(colour);
    cout << " G:" << get<GREEN_IDX>(colour);
    cout << " B:" << get<BLUE_IDX>(colour);
    cout << " A:" << get<ALPHA_IDX>(colour) << endl;
}

void printColourVector(vector<Colour> colours) {
    if (colours.size() == 0) {
        cout << "EMPTY COLOUR VECTOR" << endl;
        return;
    }
    for (Colour c : colours) {
        printColour(c);
    }
}