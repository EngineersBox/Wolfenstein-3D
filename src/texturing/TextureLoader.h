#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>

#include <map>
#include <string>
#include <vector>

#include "texture.h"
#include "../raytracer/Raytracer.h"

#define MAX_TEXTURE_AMT 20

class TextureLoader {
    public:
        TextureLoader();
        TextureLoader(std::vector<std::string>* filenames);
        ~TextureLoader();

        map<std::string, Texture>* loadTextures();

        vector<std::string>* filenames;
    private:
        vector<std::string>* getTextureFileNames();
        bool verifyFileExistance(const std::string& filename);
        bool hasExt(const std::string& filename, const std::string& extension);
        std::string stripExt(const std::string& filename, const std::string& extension);
};

TextureLoader::TextureLoader(){
    this->filenames = getTextureFileNames();
};

TextureLoader::TextureLoader(vector<std::string>* filenames) {
    this->filenames = filenames;
};

TextureLoader::~TextureLoader(){};

bool TextureLoader::hasExt(const std::string& filename, const std::string& extension) {
    return filename.substr(filename.find(".")) == extension;
}

std::string TextureLoader::stripExt(const std::string& filename, const std::string& extension) {
    if (!hasExt(filename, extension)) {
        throw FileFormatError("[" + filename + "] Has different extension: ", extension);
    }
    return filename.substr(0, filename.find("."));
}

vector<std::string>* TextureLoader::getTextureFileNames() {
    DIR* dirp = opendir(std::string("resources/textures").c_str());
    struct dirent* dp;
    vector<std::string> files;
    int texCount = 0;
    while ((dp = readdir(dirp)) != NULL && texCount < MAX_TEXTURE_AMT) {
        string cfile = std::string(dp->d_name);
        if (cfile.size() < 3 || !hasExt(cfile, ".bmp")) {
            continue;
        }
        files.push_back(dp->d_name);
        texCount++;
    }
    closedir(dirp);
    if (texCount == MAX_TEXTURE_AMT) {
        cout << "WARNING: Maximum texture import count reached [" << MAX_TEXTURE_AMT << "]" << endl;
    }
    return new vector<std::string>(files);
};

map<std::string, Texture>* TextureLoader::loadTextures() {
    int textureCount = this->filenames->size();
    map<std::string, Texture> textures;
    for (int i = 0; i < textureCount; i++) {
        const std::string& fname = this->filenames->at(i);
        if (!verifyFileExistance(fname)) {
            continue;
        }
        textures.insert(pair<string, Texture>(stripExt(fname, ".bmp"), Texture("resources/textures/" + fname, fname)));
    }
    return new map<std::string, Texture>(textures);
}

bool TextureLoader::verifyFileExistance(const std::string& filename) {
    struct stat buffer;
    if (stat(std::string("resources/textures/" + filename).c_str(), &buffer) != 0) {
        cout << "TextureLoader: [" + filename + "] File does not exist, skipping." << endl;
        return false;
    }
    return true;
}