#pragma once

#if _WIN64 || _WIN32
    #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#include <vector>
#include <string>

#include "../Globals.hpp"
#include "../colour/Colours.hpp"
#include "../../exceptions/buffering/BufferIndexOutOfRange.hpp"

using namespace std;

namespace Rendering {

#define PBO_DEFAULT_SIZE 4096
#define PBO_DEFAULT_WIDTH 64
#define PBO_DEFAULT_HEIGHT 64

class PBO {
    public:
        PBO();
        PBO(int width, int height);

        void init();
        inline void pushBufferToGPU();

        inline void pushToBuffer(int x, int y, Colour::ColorRGB colour);
        inline void resize(int newWidth, int newHeight);
    private:
        bool inRange(int value, int min, int max);

        vector<GLubyte> buffer;
        GLuint buffer_id;

        int width;
        int height;
};

PBO::PBO(){};

PBO::PBO(int width, int height) {
    this->width = width;
    this->height = height;
    this->buffer = vector<GLubyte>(width * height * 3);
    debugContext.logApiInfo("Initialised new PBO of size " + to_string(width * height * 3) + " [" + to_string(width) + "*" + to_string(height) + "*3" + "]");
};

void PBO::init() {
    glGenTextures(1, &this->buffer_id);
    glBindTexture(GL_TEXTURE_2D, this->buffer_id);
    debugContext.logApiInfo("Bound PBO texture to id: " + to_string(this->buffer_id));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->buffer.data());
    debugContext.logApiInfo("Allocated PBO texture to from: " + ADDR_OF(*this->buffer.data()));
};

inline void PBO::pushBufferToGPU() {
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT);

    glTexSubImage2D(GL_TEXTURE_2D,
        0, 0, 0, this->width,this->height,
        GL_RGB, GL_UNSIGNED_BYTE,
        this->buffer.data());

    // Inverted quad to handle pixel location inversion
    glBegin(GL_QUADS);
        glTexCoord2d(1,1); glVertex2d(0, 0);
        glTexCoord2d(0,1); glVertex2d(this->width, 0);
        glTexCoord2d(0,0); glVertex2d(this->width, this->height);
        glTexCoord2d(1,0); glVertex2d(0, this->height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

bool PBO::inRange(int value, int min, int max) {
    return !(value < min) && !(max < value);
}

inline void PBO::pushToBuffer(int x, int y, Colour::ColorRGB colour) {
    int idx = y * this->width + x;
    if (!inRange(x, 0, this->width) || !inRange(y, 0, this->height)) {
        throw BufferIndexOutOfRange(idx);
    }
    this->buffer[3 * idx + 0] = colour.r;
    this->buffer[3 * idx + 1] = colour.g;
    this->buffer[3 * idx + 2] = colour.b;
}

inline void PBO::resize(int newWidth, int newHeight) {
    this->width = newWidth;
    this->height = newHeight;
    this->buffer.resize(newWidth * newHeight * 3);
    debugContext.logApiInfo("Resized PBO to " + to_string(newWidth * newHeight * 3) + " [" + to_string(newWidth) + "*" + to_string(newHeight) + "*3" + "]");
}

}