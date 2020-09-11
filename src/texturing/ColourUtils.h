#pragma once

#include "TextureColours.h"
#include <functional>

template <typename T>
constexpr T PW_add(T a, T b)
{
    return a + b;
}

template <typename T>
constexpr T PW_sub(T a, T b)
{
    return a - b;
}

template <typename T>
constexpr T PW_mul(T a, T b)
{
    return a * b;
}

template <typename T>
constexpr T PW_div(T a, T b)
{
    return a / b;
}

///
/// Apply a piece-wise mask to a given colour via a combination method
///
/// @param Colour<GLdouble, GLdouble, GLdouble, GLdouble> base: Colour to apply to
/// @param Colour<GLdouble, GLdouble, GLdouble, GLdouble> mask: Mask to apply to colour
/// @param std::function<T(T, T)>: An arithmetic lin_operator, piece-wise operator
///
/// @return Colour<GLdouble, GLdouble, GLdouble, GLdouble>
///
template <typename T>
Colour colourMask(Colour base, Colour mask, std::function<T(T, T)> lin_operator)
{
    return {
        lin_operator(std::get<RED_IDX>(base), std::get<RED_IDX>(mask)),
        lin_operator(std::get<GREEN_IDX>(base), std::get<GREEN_IDX>(mask)),
        lin_operator(std::get<BLUE_IDX>(base), std::get<BLUE_IDX>(mask)),
        lin_operator(std::get<ALPHA_IDX>(base), std::get<ALPHA_IDX>(mask))};
}