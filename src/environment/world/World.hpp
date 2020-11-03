#pragma once

#include <vector>

#include "../base/entity/IEntityBase.hpp"
#include "../base/object/IObjectBase.hpp"
#include "../map/map.hpp"
#include "../player/Player.hpp"
#include "../../rendering/Globals.hpp"

using namespace std;

#define WORLD_MAX_ENTITY_COUNT 128
#define WORLD_MAX_OBJECT_COUNT 128

struct World {
    World();
    World(GameMap map);
    ~World();

    bool addEntity(BaseInterface::IEntityBase<> entity);
    bool removeEntity(BaseInterface::IEntityBase<> entity);
    bool addObject(BaseInterface::IObjectBase<> object);
    bool removeObject(BaseInterface::IObjectBase<> object);

    GameMap map;
    vector<BaseInterface::IEntityBase<>> entities;
    vector<BaseInterface::IObjectBase<>> objects;

    template <typename T>
    inline bool searchEraseVector(vector<T> &vec, T element);
};

World::World() {};

World::World(GameMap map) {
    this->map = map;
};

World::~World() {
};

template <typename T>
inline bool World::searchEraseVector(vector<T> &vec, T element) {
    for (int i = vec.size() - 1; i != -1; i--) {
        if (vec[i] == element) {
            vec.erase(vec.begin() + i);
            return true;
        }
    }
    return false;
}

bool World::addEntity(BaseInterface::IEntityBase<> entity) {
    if (this->entities.size() == WORLD_MAX_ENTITY_COUNT) {
        return false;
    }
    this->entities.push_back(entity);
    return true;
};

bool World::removeEntity(BaseInterface::IEntityBase<> entity) {
    if (this->entities.size() < 1) {
        return false;
    }
    return searchEraseVector<BaseInterface::IEntityBase<>>(this->entities, entity);
};

bool World::addObject(BaseInterface::IObjectBase<> object) {
    if (this->objects.size() == WORLD_MAX_OBJECT_COUNT) {
        return false;
    }
    this->objects.push_back(object);
    return true;
};

bool World::removeObject(BaseInterface::IObjectBase<> object) {
    if (this->objects.size() < 1) {
        return false;
    }
    return searchEraseVector<BaseInterface::IObjectBase<>>(this->objects, object);
};