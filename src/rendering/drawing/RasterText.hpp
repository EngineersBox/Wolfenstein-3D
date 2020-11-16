#pragma once

#if _WIN64 || _WIN32
    #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#include "../colour/Colours.hpp"
#include <string>

using namespace std;

void displayText(float x, float y, Colour::ColorRGB text_colour, void* font, const string& string) {
    GLint current_colour[4];
    glGetIntegerv(GL_CURRENT_COLOR, current_colour);
    text_colour.toColour4d();
    glRasterPos2f(x, y);
    for (int i = 0; i < string.length(); i++) {
        glutBitmapCharacter(font, string[i]);
    }
    glColor3i(
        current_colour[0],
        current_colour[1],
        current_colour[2]
    );
}

void displayTextStroke(float x, float y, double scale, Colour::ColorRGB text_colour, const string& string) {
    GLint current_colour[4];
    glGetIntegerv(GL_CURRENT_COLOR, current_colour);
    text_colour.toColour4d();

    glPushMatrix();
    glTranslatef(x, y, 0);
    gluOrtho2D(0, 1, 1, 0);
    glScaled(scale, scale, scale);
    for (int i = 0; i < string.length(); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
    }
    glPopMatrix();
    
    glColor3i(
        current_colour[0],
        current_colour[1],
        current_colour[2]);
}