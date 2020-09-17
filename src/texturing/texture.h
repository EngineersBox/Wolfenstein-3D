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

int imageLoad(const char *filename, Image *image) {
    FILE *file;
    unsigned long size;         // size of the image in bytes.
    unsigned long i;            // standard counter.
    unsigned short int planes;  // number of planes in image (must be 1)
    unsigned short int bpp;     // number of bits per pixel (must be 24)
    char temp;                  // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb")) == NULL) {
        throw LoadImageError(filename, ImageCause::FILEEXISTS, (unsigned short)NULL);
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->width, 4, 1, file)) != 1) {
        throw LoadImageError(filename, ImageCause::WIDTH, image->width);
        return 0;
    }

    // read the height
    if ((i = fread(&image->height, 4, 1, file)) != 1) {
        throw LoadImageError(filename, ImageCause::HEIGHT, image->height);
        return 0;
    }

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->width * image->height * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        throw LoadImageError(filename, ImageCause::PLANE, planes);
        return 0;
    }
    if (planes != 1) {
        throw LoadImageError("is not ", filename, ImageCause::PLANE, 1);
        return 0;
    }

    // read the bitsperpixel
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        throw LoadImageError(filename, ImageCause::BPP, bpp);
        return 0;
    }
    if (bpp != 24) {
        throw LoadImageError("is not ", filename, ImageCause::BPP, 24);
        return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    // read the data.
    image->data = (char *) malloc(size);

    if (image->data == NULL) {
        throw MemoryError("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
        throw LoadImageError(filename, ImageCause::IDATA, (unsigned short)0);
        return 0;
    }
    for (i = 0; i < size; i += 3) {  // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i + 2];
        image->data[i + 2] = temp;
    }

    // we're done.
    return 1;
}

Texture::Texture() {}

Texture::Texture(string filename, string name, int width, int height) {
    this->name = name;
    this->filename = filename;
    this->width = width;
    this->height = height;
    this->texture = BMP(filename.c_str());
}

Texture::~Texture() {}

Image* Texture::loadTexture() {
    Image *image = (Image *) malloc(sizeof(Image));

    if (image == NULL) {
        throw MemoryError("Error allocating space for image");
        exit(0);
    }

    if (!imageLoad((this->filename).c_str(), image)) {
        exit(1);
    }

    return image;
}

long long Texture::hashCode() {
    // Hashing using rolling polynomial method
    string texString = name;
    texString.append(to_string(width));
    texString.append(to_string(height));
    // for (int x = 0; x < texture.GetNumRows(); x++) {
    //     for (int y = 0; y < texture.GetNumCols(); y++) {
    //         texString.append("t");
    //         // texString.append(to_string((int)texture.GetRgbPixel(x, y)));
    //     }
    // }

    long long hashVal = 0;
    for (int i = 0; i < texString.length(); i++) {
        hashVal = (hashVal * PRIME_BASE) + texString[i];
        hashVal %= (int)PRIME_MOD;
    }
    return hashVal;
}