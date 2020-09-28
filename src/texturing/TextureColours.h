#pragma once

#if _WIN64
    #include <windows.h>
    #include <GL/glut.h>
#elif _WIN32
   #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/GL.h>
    #include <OpenGL/GLU.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#include <tuple>
#include <string>
#include <vector>
#include <functional>

using namespace std;
template<typename R, typename G, typename B, typename A>
using CTuple = tuple<R, G, B, A>;
using Colour = CTuple<GLdouble, GLdouble, GLdouble, GLdouble>;

// ---- DEFAULT COLOURS ---
Colour RED = {1.0, 0.0, 0.0, 1.0};
Colour YELLOW = {1.0, 1.0, 0.0, 1.0};
Colour GREEN = {0.0, 1.0, 0.0, 1.0};
Colour BLUE = {0.0, 0.0, 1.0, 1.0};
Colour BLACK = {0.0, 0.0, 0.0, 1.0};
Colour WHITE = {1.0, 1.0, 1.0, 1.0};
Colour DARK_GREY = {0.3, 0.3, 0.3, 1.0};
Colour LIGHT_GREY = {0.7, 0.7, 0.7, 1.0};
Colour NONE = {-1.0, -1.0, -1.0, 1.0};

// ---- CTuple INDEXES
constexpr int RED_IDX = 0;
constexpr int GREEN_IDX = 1;
constexpr int BLUE_IDX = 2;
constexpr int ALPHA_IDX = 3;

// ---- COLOUR COMPONENT RETRIEVERS ----
#define GET_RED(c) get<RED_IDX>(c)
#define GET_GREEN(c) get<GREEN_IDX>(c)
#define GET_BLUE(c) get<BLUE_IDX>(c)
#define GET_ALPHA(c) get<ALPHA_IDX>(c)

///
/// Render colour with OpenGL glColor3d
///
/// @param Colour texColour: Tuple of <R,G,B,A> values for colour
///
/// @return void
///
inline void toColour(Colour texColour) {
    glColor4d(
        GET_RED(texColour),
        GET_GREEN(texColour),
        GET_BLUE(texColour),
        GET_ALPHA(texColour)
    );
}

///
/// Render colour with OpenGL glClearColour
///
/// @param Colour texColour: Tuple of <R,G,B,A> values for colour
///
/// @return void
///
inline void toClearColour(Colour texColour) {
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
    } else if (colour == "LIGHT_GREY") {
        return LIGHT_GREY;
    } else if (colour == "DARK_GREY") {
        return DARK_GREY;
    } else {
        return NONE;
    }
}

void printColour(Colour colour) {
    if (sizeof colour == 0) {
        cout << "EMPTY COLOUR" << endl;
        return;
    }
    cout << "R:" << GET_RED(colour);
    cout << " G:" << GET_GREEN(colour);
    cout << " B:" << GET_BLUE(colour);
    cout << " A:" << GET_ALPHA(colour) << endl;
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

template <typename T>
using PWOperator = function<T(T, T)>;

template <typename T>
constexpr T PW_add(T a, T b) {
    return a + b;
}

template <typename T>
constexpr T PW_sub(T a, T b) {
    return a - b;
}

template <typename T>
constexpr T PW_mul(T a, T b) {
    return a * b;
}

template <typename T>
constexpr T PW_div(T a, T b) {
    return a / b;
}

///
/// Apply a piece-wise mask to a given colour via a combination method
///
/// @param Colour<GLdouble, GLdouble, GLdouble, GLdouble> base: Colour to apply to
/// @param Colour<GLdouble, GLdouble, GLdouble, GLdouble> mask: Mask to apply to colour
/// @param std::function<T(T, T)> lin_operator: An arithmetic piece-wise operator
///
/// @return Colour<GLdouble, GLdouble, GLdouble, GLdouble>
///
template <typename T>
Colour colourMask(Colour base, Colour mask, PWOperator<T> lin_operator) {
    return {
        lin_operator(GET_RED(base), GET_RED(mask)),
        lin_operator(GET_GREEN(base), GET_GREEN(mask)),
        lin_operator(GET_BLUE(base), GET_BLUE(mask)),
        lin_operator(GET_ALPHA(base), GET_ALPHA(mask))
    };
}