#include <string>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

using namespace std;

#define PRIME_MOD 1e9 - 9
#define PRIME_BASE 1

class Texture {
    public:
        Texture(string filename, string name, int width, int height);
        ~Texture();
        long long hashCode();

        string name;
        GLuint texture;
        int width;
        int height;
};

// From: https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
GLuint loadRawTexture(const char *filename, int width, int height) {
    GLuint texture;
    unsigned char *data;
    FILE *file;

    // open texture data
    file = fopen(filename, "rb");
    if (file == NULL) return 0;

    // allocate buffer
    data = (unsigned char*)malloc(width * height * 4);

    // read texture data
    fread(data, width * height * 4, 1, file);
    fclose(file);

    // allocate a texture name
    glGenTextures(1, &texture);

    // select our current texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture should tile
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // build our texture mipmaps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // free buffer
    free(data);
    return texture;
}

Texture::Texture(string filename, string name, int width, int height) {
    this->name = name;
    this->texture = loadRawTexture(filename.c_str(), width, height);
    this->width = width;
    this->height = height;
}

Texture::~Texture(){}

long long Texture::hashCode() {
    // Hashing using rolling polynomial method
    string texString = name;
    texString.append(to_string(width));
    texString.append(to_string(height));
    texString.append(to_string(texture));

    long long hashVal = 0;
    for (int i = 0; i < texString.length(); i++) {
        hashVal = (hashVal * PRIME_BASE) + texString[i];
        hashVal %= (int) PRIME_MOD;
    }
    return hashVal;
}