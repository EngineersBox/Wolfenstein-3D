#pragma once

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <string>
#include "BMPreader.h"

using namespace std;

#define PRIME_MOD 1e9 - 9
#define PRIME_BASE 1

#define CM_WIDTH 64
#define CM_HEIGHT 64

typedef struct Image {
    unsigned long width;
    unsigned long height;
    char *data;
} Image;

class Texture {
    public:
        Texture();
        Texture(string filename, string name, int width, int height);
        ~Texture();
        Image* loadTexture();
        long long hashCode();

        string name;
        string filename;
        BMP texture;
        int width;
        int height;
};

Texture::Texture() {}

Texture::Texture(string filename, string name, int width, int height) {
    this->name = name;
    this->filename = filename;
    this->width = width;
    this->height = height;
    this->texture = BMP(filename.c_str());
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