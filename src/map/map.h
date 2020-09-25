#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../environment/Walls.h"
#include "../exceptions/map/MapFormatError.h"
#include "../logging/GLDebug.h"
#include "../io/JSONParser.h"
#include "../raytracer/Globals.h"

using namespace std;
#define MAP_DELIM ";"

class GameMap {
    private:
    public:
        GameMap(vector<Wall> walls, int width, int height);
        GameMap();
        ~GameMap();

        void fromArray(Wall walls[], int width, int height);
        void readMapFromFile(string filename);
        void readMapFromJSON(string filename);
        Wall getAt(int x, int y);
        Wall getAtPure(int loc);
        
        int map_width;
        int map_height;
        int size;
        int wall_width;
        int wall_height;
        vector<Wall> _walls;
};

GameMap::GameMap(vector<Wall> walls, int width, int height) {
    this->map_width = width;
    this->map_width = height;
    this->_walls = walls;
    this->size = width * height;
}

GameMap::GameMap(): GameMap(vector<Wall>(0), 0, 0){};

GameMap::~GameMap(){}

void GameMap::fromArray(Wall walls[], int width, int height) {
    this->map_width = width;
    this->map_width = height;
    for (int i = 0; i < width * height; i++) {
        this->_walls.at(i) = walls[i];
    }
    this->size = width * height;
}

vector<string> splitString(string s, int substring_count, string delimiter) {
    size_t pos = 0;
    int idx = 0;
    vector<string> ss(substring_count);
    while ((pos = s.find(delimiter)) != string::npos) {
        ss.at(idx) = s.substr(0, pos);
        idx++;
        s.erase(0, pos + delimiter.length());
    }
    ss.at(idx) = s;
    return ss;
}

void GameMap::readMapFromFile(string filename) {
    ifstream inFile(filename);
    int mapFormat;
    inFile >> this->map_width >> this->map_height >> mapFormat;
    if (mapFormat != 0 && mapFormat != 1) {
        MapFormatError err(mapFormat);
        if (debugContext.l_cfg->map_skip_invalid) {
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_API,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_LOW,
                string(err.what())
            );
        } else {
            throw err;
        }
    }
    this->_walls = vector<Wall>(this->map_width * this->map_height);

    for (int y = 0; y < this->map_height; y++) {
        string currentEntry;
        inFile >> currentEntry;
        vector<string> s = splitString(currentEntry, this->map_width, MAP_DELIM);
        for (int x = 0; x < this->map_width; x++) {
            string cToken = s.at(x);
            if (mapFormat == 1) {
                this->_walls.at((y * this->map_width) + x) = Wall(x, y, toTexColour(cToken));
                continue;
            }
            this->_walls.at((y * this->map_width) + x) = Wall(x, y, cToken == "NONE" ? NONE : WHITE, cToken);
        }
    }
    this->size = this->map_width * this->map_height;
    inFile.close();
};

void GameMap::readMapFromJSON(string filename) {
    RSJresource jsonres;
    filebuf fileBuffer;
    if (fileBuffer.open(filename.c_str(), ios::in)) {
        istream is(&fileBuffer);
        jsonres = RSJresource(is);
        fileBuffer.close();
    } else {
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_OS_X_SYSTEM,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            "Unable to load map file: " + filename
        );
    }
    debugContext.logAppInfo("Retrieved map file: " + string(filename));
    debugContext.logAppInfo("---- STARTED MAP PROCESSING [" + filename +"] ----");
    this->map_width = jsonres["Params"]["Width"].as<int>();
    this->map_height = jsonres["Params"]["Height"].as<int>();
    debugContext.logAppInfo("Loading map with dimensions: W = " + to_string(this->map_width) + ", H = " + to_string(this->map_height));
    this->size = this->map_height * this->map_width;
    RSJarray wallarr = jsonres["Walls"].as_array();
    for (RSJresource wallObj : wallarr) {
        int x = wallObj["x"].as<int>();
        int y = wallObj["y"].as<int>();
        RSJobject left = wallObj["Left"].as<RSJobject>();
        RSJobject right = wallObj["Right"].as<RSJobject>();
        RSJobject up = wallObj["Up"].as<RSJobject>();
        RSJobject down = wallObj["Down"].as<RSJobject>();
        this->_walls.push_back(Wall(
                x, y,
                toTexColour(left["Colour"].as<string>()),
                WallFace(
                    toTexColour(left["Colour"].as<string>()),
                    left["Texture"].as<string>()
                ),
                WallFace(
                    toTexColour(right["Colour"].as<string>()),
                    right["Texture"].as<string>()
                ),
                WallFace(
                    toTexColour(up["Colour"].as<string>()),
                    up["Texture"].as<string>()
                ),
                WallFace(
                    toTexColour(down["Colour"].as<string>()),
                    down["Texture"].as<string>()
                )
            )
        );
    }
    debugContext.logAppInfo("Processed in " + to_string(wallarr.size()) + " wall objects");
    debugContext.logAppInfo("---- FINISHED MAP PROCESSING [" + filename + "] ----");
};

Wall GameMap::getAt(int x, int y) {
    return this->_walls.at((y * this->map_width) + x);
};

Wall GameMap::getAtPure(int loc) {
    return this->_walls.at(loc);
};