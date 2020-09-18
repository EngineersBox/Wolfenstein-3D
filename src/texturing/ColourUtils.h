#pragma once

#include <functional>

#include "TextureColours.h"

using namespace std;
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
template<class T>
Colour colourMask(Colour base, Colour mask, PWOperator<T> lin_operator) {
    return {
        lin_operator(GET_RED(base), GET_RED(mask)),
        lin_operator(GET_GREEN(base), GET_GREEN(mask)),
        lin_operator(GET_BLUE(base), GET_BLUE(mask)),
        lin_operator(GET_ALPHA(base), GET_ALPHA(mask))
    };
}