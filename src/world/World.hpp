#pragma once

#include <vector>

#include "../player/Player.hpp"
#include "../map/map.hpp"
#include "../entity/IEntityBase.hpp"
#include "../objects/IObjectBase.hpp"
#include "../raytracer/Globals.hpp"

using namespace std;

#define WORLD_MAX_ENTITY_COUNT 128
#define WORLD_MAX_OBJECT_COUNT 128

class World {
    public:
        World();
        World(GameMap *map);
        ~World();

        bool addEntity(IEntityBase<> *entity);
        bool removeEntity(IEntityBase<> *entity);
        bool addObject(IObjectBase<> *object);
        bool removeObject(IObjectBase<> *object);

        GameMap* getMap() const;
        vector<IEntityBase<>*> getEntities() const;
        IEntityBase<>* getEntityByID(int_id entity_id);
        vector<IObjectBase<>*> getObjects() const;
        IObjectBase<>* getObjectByID(int_id object_id);
    private:
        GameMap *map;
        vector<IEntityBase<>*> entities;
        vector<IObjectBase<>*> objects;

        template <typename T>
        inline bool searchEraseVector(vector<T *> &vec, T *element);
};

World::World() {
    this->map = nullptr;
};

World::World(GameMap *map) {
    this->map = map;
};

World::~World() {
    delete map;
    for (int i = this->entities.size() - 1; i != -1; i--) {
        delete this->entities[i];
    }
    for (int i = this->objects.size() - 1; i != -1; i--) {
        delete this->objects[i];
    }
};

template <typename T>
inline bool World::searchEraseVector(vector<T *> &vec, T *element) {
    for (int i = vec.size() - 1; i != -1; i--) {
        if (vec[i] == element) {
            vec.erase(vec.begin() + i);
            return true;
        }
    }
    return false;
}

bool World::addEntity(IEntityBase<> *entity) {
    if (this->entities.size() == WORLD_MAX_ENTITY_COUNT) {
        return false;
    }
    this->entities.push_back(entity);
    return true;
};

bool World::removeEntity(IEntityBase<> *entity) {
    if (this->entities.size() < 1) {
        return false;
    }
    return searchEraseVector<IEntityBase<>>(this->entities, entity);
};

bool World::addObject(IObjectBase<> *object) {
    if (this->objects.size() == WORLD_MAX_OBJECT_COUNT) {
        return false;
    }
    this->objects.push_back(object);
    return true;
};

bool World::removeObject(IObjectBase<> *object) {
    if (this->objects.size() < 1) {
        return false;
    }
    return searchEraseVector<IObjectBase<>>(this->objects, object);
};

GameMap* World::getMap() const {
    return this->map;
};

vector<IEntityBase<>*> World::getEntities() const {
    return this->entities;
};

IEntityBase<>* World::getEntityByID(int_id entity_id) {
    for (IEntityBase<>* entity : this->entities) {
        if (entity->ent_id == entity_id) {
            return entity;
        }
    }
    return nullptr;
}

vector<IObjectBase<>*> World::getObjects() const {
    return this->objects;
};

IObjectBase<>* World::getObjectByID(int_id object_id) {
    for (IObjectBase<> *object : this->objects) {
        if (object->obj_id == object_id) {
            return object;
        }
    }
    return nullptr;
}