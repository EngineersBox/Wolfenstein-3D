#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>

#include <map>
#include <string>
#include <vector>

#include "texture.hpp"
#include "../exceptions/textureLoader/ExceededMaxTextureImport.hpp"
#include "../hashing/HashTable.hpp"
#include "../raytracer/Globals.hpp"

#define MAX_TEXTURE_AMOUNT 20

// TODO: Change loader to iterate through all files in resources/textures/*
class TextureLoader {
    public:
        TextureLoader();
        TextureLoader(std::vector<std::string>* filenames);
        ~TextureLoader();

        void loadTextures(HashTable<Texture>& textures);
        std::vector<std::string>* filenames;
    private:
        std::vector<std::string>* getTextureFileNames();
        bool verifyFileExistance(const std::string& filename);
        bool hasExt(const std::string& filename, const std::string& extension) const;
        std::string stripExt(const std::string& filename, const std::string& extension);
};

TextureLoader::TextureLoader(){
    this->filenames = getTextureFileNames();
};

TextureLoader::TextureLoader(std::vector<std::string>* filenames) {
    this->filenames = filenames;
};

TextureLoader::~TextureLoader(){};

bool TextureLoader::hasExt(const std::string& filename, const std::string& extension) const {
    return filename.substr(filename.find(".")) == extension;
}

std::string TextureLoader::stripExt(const std::string& filename, const std::string& extension) {
    if (!hasExt(filename, extension)) {
        throw FileFormatError("[" + filename + "] Has different extension: ", extension);
    }
    return filename.substr(0, filename.find("."));
}

std::vector<std::string>* TextureLoader::getTextureFileNames() {
    DIR* dir = opendir(std::string("resources/textures").c_str());
    struct dirent* dp;
    std::vector<std::string> files;
    int texCount = 0;
    while ((dp = readdir(dir)) != NULL && texCount < MAX_TEXTURE_AMOUNT) {
        std::string cfile(dp->d_name);
        if (cfile.size() < 3 || !hasExt(cfile, ".bmp")) {
            continue;
        }
        files.push_back(dp->d_name);
        texCount++;
    }
    closedir(dir);
    if (texCount == MAX_TEXTURE_AMOUNT) {
        throw ExceededMaxTextureImport(MAX_TEXTURE_AMOUNT);
    }
    return new std::vector<std::string>(files);
};

void TextureLoader::loadTextures(HashTable<Texture> &textures) {
    int textureCount = this->filenames->size();
    for (int i = 0; i < textureCount; i++) {
        const std::string& fname = this->filenames->at(i);
        if (!verifyFileExistance(fname)) {
            continue;
        }
        string stripped_fname = fname.substr(0, fname.find_first_of("."));
        textures.insert(stripped_fname, Texture("resources/textures/" + fname, stripped_fname));
    }
}

bool TextureLoader::verifyFileExistance(const std::string& filename) {
    struct stat buffer;
    if (stat(std::string("resources/textures/" + filename).c_str(), &buffer) != 0) {
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_SYSTEM,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_LOW,
            "[" + filename + "] File does not exist, skipping."
        );
        return false;
    }
    return true;
}