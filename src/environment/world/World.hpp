#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include "../constructs/walls//AABBFace.hpp"
#include "../constructs/walls/AABB.hpp"
#include "../../exceptions/map/MapFormatError.hpp"
#include "../../io/resource_management/JSONParser.hpp"
#include "../../io/logging/GLDebug.hpp"
#include "Coordinates.hpp"
#include "../../rendering/Globals.hpp"
#include "../constructs/sprites/Sprite.hpp"
#include "../constructs/sprites/Enemy.hpp"
#include "../../rendering/colour/Colours.hpp"

using namespace std;
#define MAP_DELIM ";"

struct World {
    World(vector<Constructs::AABB> walls, int width, int height, GLDebugContext* context = &debugContext);
    World(GLDebugContext *context = &debugContext);

    void fromArray(Constructs::AABB walls[], int width, int height);
    void readMapFromJSON(string filename);
    Constructs::AABB getAt(int x, int y);
    Constructs::AABB getAtPure(int loc);

    inline double sqDist(double ax, double ay, double bx, double by);
    inline void sortSprites(Coordinates<double> player_loc);
    void updateSprites();

    int map_width;
    int map_height;
    int size;
    int wall_width;
    int wall_height;

    Coords start;
    Coords end;

    vector<Constructs::AABB> walls;
    vector<Coords> up_boundary;
    vector<Coords> down_boundary;
    vector<Coords> left_boundary;
    vector<Coords> right_boundary;
    vector<Coords> tree_order_walls;
    vector<Constructs::Sprite> sprites;

    string ceiling_texture;
    string floor_texture;

    GLDebugContext* context;
};

World::World(vector<Constructs::AABB> walls, int width, int height, GLDebugContext *context) {
    this->map_width = width;
    this->map_width = height;
    this->walls = walls;
    this->size = width * height;
    this->context = context;
}

World::World(GLDebugContext *context){
    this->walls = vector<Constructs::AABB>(0);
    this->map_width = 0;
    this->map_height = 0;
    this->size = 0;
    this->context = context;
};

void World::fromArray(Constructs::AABB walls[], int width, int height) {
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

void World::readMapFromJSON(string filename) {
    ResourceManager::RSJresource jsonres;
    filebuf fileBuffer;
    if (fileBuffer.open(filename.c_str(), ios::in)) {
        istream is(&fileBuffer);
        jsonres = ResourceManager::RSJresource(is);
        fileBuffer.close();
    } else {
        this->context->glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_SYSTEM,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            "Unable to load map file: " + filename
        );
    }
    this->context->logSysInfo("Retrieved map file: " + filename);
    this->context->logAppInfo("---- STARTED MAP PROCESSING [" + filename +"] ----");
    this->map_width = jsonres["Params"]["Width"].as<int>();
    this->map_height = jsonres["Params"]["Height"].as<int>();
    this->context->logAppInfo("Loading map with dimensions: W = " + to_string(this->map_width) + ", H = " + to_string(this->map_height));
    this->size = this->map_height * this->map_width;
    this->start = Coords(jsonres["Params"]["Start"]["x"].as<int>(),jsonres["Params"]["Start"]["y"].as<int>());
    this->end = Coords(jsonres["Params"]["End"]["x"].as<int>(),jsonres["Params"]["End"]["y"].as<int>());
    this->context->logAppInfo("Map start location: " + this->start.asString());
    this->context->logAppInfo("Map end location: " + this->end.asString());
    ResourceManager::RSJarray wallarr = jsonres["Walls"].as_array();
    for (ResourceManager::RSJresource wallObj : wallarr) {
        int x = wallObj["x"].as<int>();
        int y = wallObj["y"].as<int>();
        ResourceManager::RSJobject left = wallObj["Left"].as<ResourceManager::RSJobject>();
        ResourceManager::RSJobject right = wallObj["Right"].as<ResourceManager::RSJobject>();
        ResourceManager::RSJobject up = wallObj["Up"].as<ResourceManager::RSJobject>();
        ResourceManager::RSJobject down = wallObj["Down"].as<ResourceManager::RSJobject>();
        this->walls.push_back(Constructs::AABB(
            x, y,
            Colour::STRtoRGB(left["Colour"].as<string>()),
            Constructs::AABBFace(
                Colour::STRtoRGB(left["Colour"].as<string>()),
                left["Texture"].as<string>()),
            Constructs::AABBFace(
                Colour::STRtoRGB(right["Colour"].as<string>()),
                right["Texture"].as<string>()),
            Constructs::AABBFace(
                Colour::STRtoRGB(up["Colour"].as<string>()),
                up["Texture"].as<string>()),
            Constructs::AABBFace(
                Colour::STRtoRGB(down["Colour"].as<string>()),
                down["Texture"].as<string>())));
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
    this->context->logAppInfo("Processed " + to_string(wallarr.size()) + " AABB objects");
    ResourceManager::RSJarray spritearr = jsonres["Sprites"].as_array();
    if (spritearr.size() > 0) {
        for (ResourceManager::RSJresource spriteObj : spritearr) {
            double x = spriteObj["x"].as<double>();
            double y = spriteObj["y"].as<double>();
            string texture = spriteObj["Texture"].as<string>();
            ResourceManager::RSJresource isEnemyObj = spriteObj["Enemy"];
            bool isEnemy = isEnemyObj.exists() ? isEnemyObj.as<bool>() : false;
            if (isEnemy) {
                ResourceManager::RSJarray animation_frames = spriteObj["Animation Frames"].as_array();
                vector<string> frames(animation_frames.size());
                for_each(animation_frames.begin(), animation_frames.end(), [&frames](ResourceManager::RSJresource frame) {
                    frames.push_back(frame.as<string>());
                });
                int tick_rate = spriteObj["Tick Rate"].as<int>();
                this->sprites.push_back(Constructs::Enemy(
                    x,y,
                    frames,
                    tick_rate
                ));
            } else {
                this->sprites.push_back(Constructs::Sprite(
                    x, y,
                    texture
                ));
            }
        }
        this->context->logAppInfo("Processed " + to_string(spritearr.size()) + " Sprite entities");
    }
    this->ceiling_texture = jsonres["Ceiling"].as<string>();
    this->context->logAppInfo("Loaded ceiling texture [" + this->ceiling_texture + "]");
    this->floor_texture = jsonres["Floor"].as<string>();
    this->context->logAppInfo("Loaded floor texture [" + this->floor_texture + "]");
    this->context->logAppInfo("---- FINISHED MAP PROCESSING [" + filename + "] ----");
};

Constructs::AABB World::getAt(int x, int y) {
    return this->walls.at((y * this->map_width) + x);
};

Constructs::AABB World::getAtPure(int loc) {
    return this->walls.at(loc);
};

double World::sqDist(double ax, double ay, double bx, double by) {
    return pow(bx - ax, 2) + pow(by - ay, 2);
};

inline void World::sortSprites(Coordinates<double> player_loc) {
    accumulate(this->sprites.begin(), this->sprites.end(), 0, [this, player_loc](int i, Constructs::Sprite& sprite) -> int {
        sprite.distance = sqDist(sprite.location.x, player_loc.x, sprite.location.y, player_loc.y);
        sprite.order = i;
        return i++;
    });
    sort(this->sprites.begin(), this->sprites.end());
};

void World::updateSprites() {
    for_each(this->sprites.begin(), this->sprites.end(), [](Constructs::Sprite &sprite){
        sprite.update();
    });
};