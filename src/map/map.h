#pragma once

#include <iostream>
#include <vector>
#include "../environment/Walls.h"
#include <fstream>

#define MAP_DELIM ";"

using namespace std;

class GameMap {
    public:
        GameMap(vector<Wall> walls, int width, int height);
        GameMap();
        ~GameMap();

        void fromArray(Wall walls[], int width, int height);
        Wall* toArray();
        void readMapFromFile(string filename);

        Wall getAt(int x, int y);
    private:
        int map_width;
        int map_height;
        vector<Wall> _walls;
};

GameMap::GameMap(vector<Wall> walls, int width, int height){
    this->map_width = width;
    this->map_width = height;
    this->_walls = walls;
}

GameMap::~GameMap(){
    delete &_walls;
}

void GameMap::fromArray(Wall walls[], int width, int height) {
    this->map_width = width;
    this->map_width = height;
    for (int i = 0; i < width * height; i++) {
        this->_walls.at(i) = walls[i];
    }
}

Wall* GameMap::toArray() {
    Wall returnArray[this->map_width * this->map_height];
    for (int i = 0; i < this->map_height; i++) {
        for (int j = 0; j < this->map_width; j++) {
            int cIdx = (i * this->map_width) + j;
            returnArray[cIdx] = this->_walls.at(cIdx);
        }
    }
    return returnArray;
}

Wall parseWall(string wall_string, string delimiter) {
    size_t pos = 0;
    string token[3];
    while((pos = wall_string.find(delimiter)) != string::npos) {
        token[pos] = wall_string.substr(0, pos);
        wall_string.erase(0, pos + delimiter.length());
    }
    return Wall(stod(token[0]), stod(token[1]), token[2]);
}

void GameMap::readMapFromFile(string filename) {
    ifstream inFile(filename);
    int width, height;
    inFile >> width >> height;
    this->map_height = height;
    this->map_width = width;
    this->_walls = vector<Wall>(width * height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            string currentEntry;
            inFile >> currentEntry;
            this->_walls.at((i * width) + j) = parseWall(currentEntry, MAP_DELIM);
        }
    }
}

Wall GameMap::getAt(int x, int y) {
    return this->_walls.at((x * this->map_width) + y);
}