#pragma once

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <string>
#include "BMPreader.h"

using namespace std;

class Texture {
    public:
        Texture();
        Texture(string filename, string name);
        ~Texture();

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