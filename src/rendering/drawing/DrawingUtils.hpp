#pragma once

#if _WIN64
    #include <windows.h>
    #include <GL/glut.h>
#elif _WIN32
   #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#include "../colour/Colours.hpp"

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
inline static void drawRectangle(float x, float y, float xSideLength, float ySideLength, bool beginEnd = true) {
    if (beginEnd) {
        glBegin(GL_QUADS);
    }

    glVertex2f(x, y);                              // Top right
    glVertex2f(x, y + ySideLength);                // Top left
    glVertex2f(x + xSideLength, y + ySideLength);  // Bottom left
    glVertex2f(x + xSideLength, y);                // Bottom right

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
inline static void drawSquare(float x, float y, float sidelength, bool beginEnd = true) {
    drawRectangle(x, y, sidelength, sidelength, beginEnd);
}

///
/// Render a line between two points (ax, ay) and (bx, by) with a given width
///
/// @param float ax: X-axis value for point A
/// @param float ay: Y-axis value for point A
/// @param float bx: X-axis value for point B
/// @param float by: Y-axis value for point B
/// @param float line_width: Width of the ray to draw
///
/// @return void
///
inline static void renderRay(float ax, float ay, float bx, float by, int line_width, Colour::ColorRGB ray_colour) {
    ray_colour.toColour4d();
    glLineWidth((float)line_width);

    glBegin(GL_LINES);

    glVertex2f(ax, ay);
    glVertex2f(bx, by);

    glEnd();
}
