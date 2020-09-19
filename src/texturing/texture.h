#pragma once

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <string>
#include "BMPreader.h"

using namespace std;

#define PRIME_MOD 1e9 - 9
#define PRIME_BASE 1

class Texture {
    public:
        Texture();
        Texture(string filename, string name);
        ~Texture();
        long long hashCode();

        string name;
        string filename;
        BMP texture;
        int width;
        int height;
};

Texture::Texture() {}

Texture::Texture(string filename, string name) {
    this->name = name;
    this->filename = filename;
    this->texture = BMP(filename.c_str());
    this->width = this->texture.bmp_info_header.width;
    this->height = this->texture.bmp_info_header.height;
}

Texture::~Texture() {}

long long Texture::hashCode() {
    // Hashing using rolling polynomial method
    string texString = name;
    texString.append(to_string(width));
    texString.append(to_string(height));
    for (uint8_t elem : texture.data) {
        texString.append(to_string(elem));
    }

    long long hashVal = 0;
    for (int i = 0; i < texString.length(); i++) {
        hashVal = (hashVal * PRIME_BASE) + texString[i];
        hashVal %= (int)PRIME_MOD;
    }
    return hashVal;
}