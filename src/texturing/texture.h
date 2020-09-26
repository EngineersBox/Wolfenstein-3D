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

#include <string>
#include "../raytracer/Globals.h"
#include "BMPreader.h"

using namespace std;

class Texture {
    public:
        Texture();
        Texture(string filename, string name);
        ~Texture();

        bool operator==(Texture& other);
        bool operator!=(Texture& other);

        string name;
        string filename;
        BMP texture;
        int width;
        int height;
};

Texture::Texture() {};

Texture::Texture(string filename, string name) {
    this->name = name;
    this->filename = filename;
    this->texture = BMP(filename.c_str());
    this->width = this->texture.bmp_info_header.width;
    this->height = this->texture.bmp_info_header.height;
};

Texture::~Texture() {};

bool Texture::operator==(Texture& other) {
    return (this->name == other.name)
        && (this->filename == other.filename)
        && (this->width == other.width)
        && (this->height == other.height)
        && (this->texture == other.texture);
}