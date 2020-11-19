#pragma once

#if _WIN64 || _WIN32
   #include <windows.h>
    #include <GL/glut.h>
#elif __APPLE__
    #include <GLUT/glut.h>
#elif __linux__
    #include <GL/glut.h>
#endif

#include <string>

#include "../Globals.hpp"
#include "../../io/resource_management/PNGReader.hpp"
#include "../../exceptions/image/ImageFileStreamError.hpp"

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
        PNGTex texture;
        unsigned long width;
        unsigned long height;
};

Texture::Texture() {};

Texture::Texture(string filename, string name) {
    this->name = name;
    this->filename = filename;
    unsigned long error = 0 | ResourceManager::PNG::loadImage(this->texture, this->width, this->height, filename.c_str());
    if (error) {
        throw ImageFileStreamError(filename);
    }
    this->texture.resize(64 * 64);
};

Texture::~Texture() {};

bool Texture::operator==(Texture& other) {
    return (this->name == other.name)
        && (this->filename == other.filename)
        && (this->width == other.width)
        && (this->height == other.height)
        && (this->texture == other.texture);
}