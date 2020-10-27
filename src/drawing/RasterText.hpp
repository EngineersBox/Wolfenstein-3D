#pragma once

#if _WIN64 || _WIN32
    #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#include "../texturing/TextureColours.hpp"
#include <string>

using namespace std;

void displayText(float x, float y, Colour text_colour, const char *string) {
    toColour(text_colour);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(string); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}