#pragma once

#include <dirent.h>
#include <sys/stat.h>
#include <iostream>

#include <map>
#include <string>
#include <vector>

#include "texture.hpp"
#include "../exceptions/image/FileFormatError.hpp"
#include "../exceptions/textureLoader/ExceededMaxTextureImport.hpp"
#include "../hashing/HashTable.hpp"
#include "../raytracer/Globals.hpp"

#define MAX_TEXTURE_AMOUNT 20

// TODO: Change loader to iterate through all files in resources/textures/*
class TextureLoader {
    public:
        TextureLoader();
        TextureLoader(std::vector<std::string>& filenames);

        void loadTextures(HashTable<Texture>& textures);
        std::vector<std::string> filenames;
    private:
        void listFiles(const string& path, std::vector<std::string>& files);
        void getTextureFileNames();
        bool verifyFileExistance(const std::string& filename);
        bool hasExt(const std::string& filename, const std::string& extension) const;
        std::string stripExt(const std::string& filename, const std::string& extension);
};

TextureLoader::TextureLoader() {
};

TextureLoader::TextureLoader(std::vector<std::string>& filenames) {
    this->filenames = std::vector<std::string>(filenames);
};

bool TextureLoader::hasExt(const std::string& filename, const std::string& extension) const {
    return filename.substr(filename.find(".")) == extension;
}

std::string TextureLoader::stripExt(const std::string& filename, const std::string& extension) {
    if (!hasExt(filename, extension)) {
        throw FileFormatError("[" + filename + "] Has different extension to: ", extension);
    }
    return filename.substr(0, filename.find("."));
}

void TextureLoader::listFiles(const string& path, std::vector<std::string>& files) {
    if (DIR* dir = opendir(path.c_str())) {
        while (dirent* f = readdir(dir)) {
            if (f->d_name[0] == '.') continue;
            if (f->d_type == DT_DIR)
                listFiles(path + f->d_name + "/", files);

            if (f->d_type == DT_REG) {
                files.push_back(path + f->d_name);
            }
        }
        closedir(dir);
    }
}

void TextureLoader::getTextureFileNames() {
    listFiles(std::string(TEX_DIR), this->filenames);
    if (this->filenames.size() == MAX_TEXTURE_AMOUNT) {
        throw ExceededMaxTextureImport(MAX_TEXTURE_AMOUNT);
    }
};

void TextureLoader::loadTextures(HashTable<Texture> &textures) {
    getTextureFileNames();
    int textureCount = this->filenames.size();
    for (int i = 0; i < textureCount; i++) {
        const std::string& fname = this->filenames.at(i);
        if (!verifyFileExistance(fname)) {
            continue;
        }
        std::string stripped_fname = fname.substr(fname.find_last_of("/") + 1, fname.size() - 1);
        std::string pure_fname = stripped_fname.substr(0, stripped_fname.find_first_of("."));
        textures.insert(pure_fname, Texture(fname, pure_fname));
        debugContext.logAppVerb("Loaded texture [" + fname + "]");
    }
}

bool TextureLoader::verifyFileExistance(const std::string& filename) {
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) != 0) {
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