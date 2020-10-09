#pragma once

#include <cmath>
#include <vector>
#include <limits>

#include "../environment/Walls.hpp"
#include "../map/map.hpp"
#include "../raytracer/Globals.hpp"
#include "../raytracer/Ray.hpp"

using namespace std;

struct GONode {
    GONode(Coords wall_coords);

    Coords wall_coords;
    GONode* infront = nullptr;
    GONode* behind = nullptr;
    GONode* parent = nullptr;
};

GONode::GONode(Coords wall_coords) {
    this->wall_coords = wall_coords;
};

class GridOrderTree {
    public:
        GridOrderTree(GameMap map, vector<Wall>* ordered_walls);
        ~GridOrderTree();

        void buildTree();
        void insertNode(GONode* root_node, GONode* node);

        vector<Coords>* queryWalls(Coords origin, vector<Ray>* rays);

        void printPreorder(GONode* node);
    private:
        bool isBehind(GONode* a, GONode* b);
        double sqDist(Coords a, Coords b);
        void findMiddle();

        GameMap map;
        vector<Wall>* walls;
        Coords mid;
        GONode* root;
};

GridOrderTree::GridOrderTree(GameMap map, vector<Wall>* ordered_walls) {
    this->map = map;
    this->walls = ordered_walls;
    this->root = new GONode(this->mid);
};

GridOrderTree::~GridOrderTree(){};

bool GridOrderTree::isBehind(GONode* a, GONode* b) {
    return (a->wall_coords.y * this->map.map_width + a->wall_coords.x)
        > (b->wall_coords.y * this->map.map_width + b->wall_coords.x);
};

// TEST: make sure this inserts nodes and links correctly
void GridOrderTree::insertNode(GONode* root_node, GONode* node) {
    if (isBehind(root_node, node)) {
        if (root_node->behind == nullptr) {
            root_node->behind = node;
            node->parent = root_node->behind;
        } else {
            insertNode(root_node->behind, node);
        }
    } else {
        if (root_node->infront == nullptr) {
            root_node->infront = node;
            node->parent = root_node->infront;
        } else {
            insertNode(root_node->infront, node);
        }
    }
    // TEST: Check if this is equivalent to the above

    // bool behind = isBehind(root_node, node);
    // GONode* behind_node = (behind ? root_node->behind : root_node->infront);
    // if (behind_node != nullptr) {
    //     insertNode(behind_node, node);
    //     return;
    // }
    // behind_node = node;
    // node->parent = behind_node;
};

double GridOrderTree::sqDist(Coords a, Coords b) {
    return pow(abs(b.x - a.x), 2) + pow(abs(b.y - a.y), 2);
};

void GridOrderTree::findMiddle() {
    Coords current_mid((int)floor(map.map_width), (int)floor(map.map_height));
    double current_min_dist = numeric_limits<double>::max();
    int current_wall_idx;
    for (int i = walls->size() - 1; i != -1; i--) {
        Coords loc = Coords(walls->at(i).posX, walls->at(i).posY);
        double dist = sqDist(this->mid, loc);
        if (dist < current_min_dist) {
            current_mid = loc;
            current_min_dist = dist;
            current_wall_idx = i;
        }
    }
    this->walls->erase(this->walls->begin() + current_wall_idx);
    this->mid = current_mid;
    this->root = new GONode(current_mid);
};

void GridOrderTree::buildTree() {
    findMiddle();
    for (Wall wall : *walls) {
        insertNode(this->root, new GONode(Coords(wall.posX, wall.posY)));
    }
};

vector<Coords>* GridOrderTree::queryWalls(Coords origin, vector<Ray>* rays) {
    // TODO: implement tree traversal and node query against rays
    vector<Coords> intersections;
    GONode *curr = this->root;
    int raysQueried = 0;
    while (raysQueried < rays->size()) {
        continue;
    }
    return new vector<Coords>(intersections);
};

void GridOrderTree::printPreorder(GONode* node) {
    if (node == NULL) {
        return;
    }
    cout << "(" << node->wall_coords.x << "," << node->wall_coords.y << ") ";
    printPreorder(node->infront);
    printPreorder(node->behind);
};