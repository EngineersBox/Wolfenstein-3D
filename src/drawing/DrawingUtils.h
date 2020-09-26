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
void drawSquare(float x, float y, float sidelength, bool beginEnd = true) {
    drawRectangle(x, y, sidelength, sidelength, beginEnd);
}