#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../environment/Walls.h"
#include "../exceptions/map/MapFormatError.h"
#include "../logging/GLDebug.h"

using namespace std;
#define MAP_DELIM ";"

class GameMap {
    private:
        GLDebugContext* debugContext;
    public:
        GameMap(vector<Wall> walls, int width, int height, GLDebugContext* debugContext);
        GameMap(GLDebugContext* debugContext);
        GameMap();
        ~GameMap();

        void fromArray(Wall walls[], int width, int height);
        void readMapFromFile(string filename);
        Wall getAt(int x, int y);
        Wall getAtPure(int loc);
        
        int map_width;
        int map_height;
        int wall_width;
        int wall_height;
        vector<Wall> _walls;
};

GameMap::GameMap(vector<Wall> walls, int width, int height, GLDebugContext* debugContext) {
    this->map_width = width;
    this->map_width = height;
    this->_walls = walls;
    this->debugContext = debugContext;
}

GameMap::GameMap(GLDebugContext* debugContext) : GameMap(vector<Wall>(0), 0, 0, debugContext){};

GameMap::GameMap() : GameMap(vector<Wall>(0), 0, 0, nullptr) {}

GameMap::~GameMap(){}

void GameMap::fromArray(Wall walls[], int width, int height) {
    this->map_width = width;
    this->map_width = height;
    for (int i = 0; i < width * height; i++) {
        this->_walls.at(i) = walls[i];
    }
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
        if (this->debugContext->l_cfg->map_skip_invalid) {
            this->debugContext->glDebugMessageCallback(
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
}

Wall GameMap::getAt(int x, int y) {
    return this->_walls.at((y * this->map_width) + x);
}

Wall GameMap::getAtPure(int loc) {
    return this->_walls.at(loc);
}