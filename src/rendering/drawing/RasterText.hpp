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

void displayText(float x, float y, Colour::ColorRGB text_colour, const string& string) {
    text_colour.toColour4d();
    glRasterPos2f(x, y);
    for (int i = 0; i < string.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}