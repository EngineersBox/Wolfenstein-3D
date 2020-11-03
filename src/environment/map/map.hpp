#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../constructs/walls//AABBFace.hpp"
#include "../constructs/walls/AABB.hpp"
#include "../../exceptions/map/MapFormatError.hpp"
#include "../../io/resource_management/JSONParser.hpp"
#include "../../io/logging/GLDebug.hpp"
#include "../map/Coordinates.hpp"
#include "../../rendering/Globals.hpp"
#include "../constructs/sprites/Sprite.hpp"
#include "../../rendering/colour/Colours.hpp"

using namespace std;
#define MAP_DELIM ";"

struct GameMap {
    GameMap(vector<AABB> walls, int width, int height);
    GameMap();

    void fromArray(AABB walls[], int width, int height);
    void readMapFromJSON(string filename);
    AABB getAt(int x, int y);
    AABB getAtPure(int loc);
    
    int map_width;
    int map_height;
    int size;
    int wall_width;
    int wall_height;

    Coords start;
    Coords end;

    vector<AABB> walls;
    vector<Coords> up_boundary;
    vector<Coords> down_boundary;
    vector<Coords> left_boundary;
    vector<Coords> right_boundary;
    vector<Coords> tree_order_walls;
    vector<Sprite> sprites;
};

GameMap::GameMap(vector<AABB> walls, int width, int height) {
    this->map_width = width;
    this->map_width = height;
    this->walls = walls;
    this->size = width * height;
}

GameMap::GameMap(){
    this->walls = vector<AABB>(0);
    this->map_width = 0;
    this->map_height = 0;
    this->size = 0;
};

void GameMap::fromArray(AABB walls[], int width, int height) {
    this->map_width = width;
    this->map_width = height;
    for (int i = 0; i < width * height; i++) {
        this->walls.at(i) = walls[i];
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

void GameMap::readMapFromJSON(string filename) {
    RSJresource jsonres;
    filebuf fileBuffer;
    if (fileBuffer.open(filename.c_str(), ios::in)) {
        istream is(&fileBuffer);
        jsonres = RSJresource(is);
        fileBuffer.close();
    } else {
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_SYSTEM,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            "Unable to load map file: " + filename
        );
    }
    debugContext.logSysInfo("Retrieved map file: " + filename);
    debugContext.logAppInfo("---- STARTED MAP PROCESSING [" + filename +"] ----");
    this->map_width = jsonres["Params"]["Width"].as<int>();
    this->map_height = jsonres["Params"]["Height"].as<int>();
    debugContext.logAppInfo("Loading map with dimensions: W = " + to_string(this->map_width) + ", H = " + to_string(this->map_height));
    this->size = this->map_height * this->map_width;
    this->start = Coords(jsonres["Params"]["Start"]["x"].as<int>(),jsonres["Params"]["Start"]["y"].as<int>());
    this->end = Coords(jsonres["Params"]["End"]["x"].as<int>(),jsonres["Params"]["End"]["y"].as<int>());
    debugContext.logAppInfo("Map start location: " + this->start.asString());
    debugContext.logAppInfo("Map end location: " + this->end.asString());
    RSJarray wallarr = jsonres["Walls"].as_array();
    for (RSJresource wallObj : wallarr) {
        int x = wallObj["x"].as<int>();
        int y = wallObj["y"].as<int>();
        RSJobject left = wallObj["Left"].as<RSJobject>();
        RSJobject right = wallObj["Right"].as<RSJobject>();
        RSJobject up = wallObj["Up"].as<RSJobject>();
        RSJobject down = wallObj["Down"].as<RSJobject>();
        this->walls.push_back(AABB(
                x, y,
                Colour::STRtoRGB(left["Colour"].as<string>()),
                AABBFace(
                    Colour::STRtoRGB(left["Colour"].as<string>()),
                    left["Texture"].as<string>()
                ),
                AABBFace(
                    Colour::STRtoRGB(right["Colour"].as<string>()),
                    right["Texture"].as<string>()
                ),
                AABBFace(
                    Colour::STRtoRGB(up["Colour"].as<string>()),
                    up["Texture"].as<string>()
                ),
                AABBFace(
                    Colour::STRtoRGB(down["Colour"].as<string>()),
                    down["Texture"].as<string>()
                )
            )
        );
        if (x == 0) {
            this->left_boundary.push_back(Coords(x,y));
        } else if (x == this->map_width - 1) {
            this->right_boundary.push_back(Coords(x,y));
        } else if (y == 0) {
            this->up_boundary.push_back(Coords(x,y));
        } else if (y == this->map_height - 1) {
            this->down_boundary.push_back(Coords(x,y));
        } else {
            this->tree_order_walls.push_back(Coords(x,y));
        }
    }
    debugContext.logAppInfo("Processed " + to_string(wallarr.size()) + " AABB objects");
    RSJarray spritearr = jsonres["Sprites"].as_array();
    for (RSJresource spriteObj : spritearr) {
        double x = spriteObj["x"].as<double>();
        double y = spriteObj["y"].as<double>();
        string texture = spriteObj["Texture"].as<string>();
        this->sprites.push_back(Sprite(
            x,
            y,
            texture
        ));
    }
    debugContext.logAppInfo("Processed " + to_string(spritearr.size()) + " Sprite entities");
    debugContext.logAppInfo("---- FINISHED MAP PROCESSING [" + filename + "] ----");
};

AABB GameMap::getAt(int x, int y) {
    return this->walls.at((y * this->map_width) + x);
};

AABB GameMap::getAtPure(int loc) {
    return this->walls.at(loc);
};