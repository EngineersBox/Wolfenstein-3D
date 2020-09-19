#pragma once

#include <GLUT/glut.h>

using namespace std;

///
/// Render a square at coodinates with top-left origin
///
/// @param int x: X coordinate
/// @param int y: Y coordinate
/// @param int xSidelength: Side length of the x-axis
/// @param int ySideLength: Side length of the y-axis
/// @param bool beginEnd: Whether to call glBegin() and glEnd()
///
/// @return void
///
void drawRectangle(float x, float y, float xSideLength, float ySideLength, bool beginEnd = true) {
    if (beginEnd) {
        glBegin(GL_QUADS);
    }

    glVertex2f(x + 1, y + 1);                              // Top right
    glVertex2f(x + 1, y + ySideLength - 1);                // Top left
    glVertex2f(x + xSideLength - 1, y + ySideLength - 1);  // Bottom left
    glVertex2f(x + xSideLength - 1, y + 1);                // Bottom right

    if (beginEnd) {
        glEnd();
    }
}

///
/// Render a square at coodinates with top-left origin
///
/// @param int x: X coordinate
/// @param int y: Y coordinate
/// @param int sidelength: Side length of the square
/// @param bool beginEnd: Whether to call glBegin() and glEnd()
///
/// @return void
///
void drawSquare(float x, float y, float sidelength, bool beginEnd = true) {
    drawRectangle(x, y, sidelength, sidelength, beginEnd);
}