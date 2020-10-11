#pragma once

#include <cmath>
#include <vector>
#include <stack>

#include "../environment/Walls.hpp"
#include "../raytracer/Globals.hpp"
#include "../raytracer/Ray.hpp"
#include "QuadNode.hpp"
#include "../map/map.hpp"

using namespace std;

enum RelativePosition : size_t {
    REL_UP = 0,
    REL_DOWN = 1,
    REL_LEFT = 2,
    REL_RIGHT = 3
};

class QSPTree {
    public:
        QSPTree();
        QSPTree(GameMap map, vector<Wall>* ordered_walls, vector<Wall>* u_bound, vector<Wall>* d_bound, vector<Wall>* l_bound, vector<Wall>* r_bound);
        ~QSPTree();

        void buildTree();
        QuadNode* insertNode(QuadNode* root, QuadNode* node);
        inline QuadNode* insertRelative(QuadNode* dir_node, QuadNode* node);

        void queryWalls(Coords origin, vector<Ray>* rays, vector<Coords>& ret_walls);

        void printPreorder(QuadNode* node);
    private:
        RelativePosition position(Coords a, Coords b);
        double sqDist(Coords a, Coords b);
        void findMiddle();

        GameMap map;
        vector<Wall>* walls;
        vector<Wall>* up_boundary;
        vector<Wall>* down_boundary;
        vector<Wall>* left_boundary;
        vector<Wall>* right_boundary;
        Coords mid;
        QuadNode* root;
};

QSPTree::QSPTree(){};

QSPTree::QSPTree(GameMap map, vector<Wall>* ordered_walls, vector<Wall>* u_bound, vector<Wall>* d_bound, vector<Wall>* l_bound, vector<Wall>* r_bound) {
    this->map = map;
    this->walls = ordered_walls;
    findMiddle();
    this->up_boundary = u_bound;
    this->down_boundary = d_bound;
    this->left_boundary = l_bound;
    this->right_boundary = r_bound;
};

QSPTree::~QSPTree() {
    delete root;
};

double QSPTree::sqDist(Coords a, Coords b) {
    int delta_x = abs(b.x - a.x);
    int delta_y = abs(b.y - a.y);
    return (delta_x * delta_x) + (delta_y * delta_y);
};

void QSPTree::findMiddle() {
    Coords current_mid((int)floor(map.map_width), (int)floor(map.map_height));
    double current_min_dist = numeric_limits<double>::max();
    int current_wall_idx;
    for (int i = walls->size() - 1; i != -1; i--) {
        Coords loc = Coords(walls->at(i).posX, walls->at(i).posY);
        int dist = sqDist(this->mid, loc);
        if (dist < current_min_dist) {
            current_mid = loc;
            current_min_dist = dist;
            current_wall_idx = i;
        }
    }
    this->walls->erase(this->walls->begin() + current_wall_idx);
    this->mid = current_mid;
    debugContext.logAppInfo("Closest wall to actuall map centre at: " + current_mid.asString());
    this->root = new QuadNode(current_mid);
};

RelativePosition QSPTree::position(Coords a, Coords b) {
    double angle = atan2(
        (b.x - a.x) + (b.y - a.y),
        (b.x - a.x) - (b.y - a.y)
    );
    if (0 <= angle && angle < M_PI_2) {
        return RelativePosition::REL_RIGHT;
    } else if (M_PI_2 <= angle && angle < M_PI) {
        return RelativePosition::REL_UP;
    } else if (M_PI <= angle && angle <= THREE_HALF_PI) {
        return RelativePosition::REL_LEFT;
    } else {
        return RelativePosition::REL_DOWN;
    }
};

inline QuadNode* QSPTree::insertRelative(QuadNode* dir_node, QuadNode* node) {
    if (dir_node == nullptr) {
        dir_node = node;
        node->parent = dir_node;
    } else {
        dir_node = insertNode(dir_node, node);
    }
    return dir_node;
};

QuadNode* QSPTree::insertNode(QuadNode* root, QuadNode* node) {
    RelativePosition relPos = position(root->wall_coords, node->wall_coords);
    if (relPos == RelativePosition::REL_UP) {
        root->U = insertRelative(root->U, node);
    } else if (relPos == RelativePosition::REL_DOWN) {
        root->D = insertRelative(root->D, node);
    } else if (relPos == RelativePosition::REL_LEFT) {
        root->L = insertRelative(root->L, node);
    } else {
        root->R = insertRelative(root->R, node);
    }
    return root;
};

void QSPTree::buildTree() {
    debugContext.logAppInfo("---- STARTED BUILDING QSP TREE ----");
    for (Wall wall : *this->walls) {
        this->root = insertNode(this->root, new QuadNode(Coords(wall.posX, wall.posY)));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->walls->size()) + " wall nodes");
    // Inserting boundaries last will garuantee them as leaf nodes
    for (Wall wall : *this->up_boundary) {
        this->root->U = insertNode(this->root->U, new QuadNode(Coords(wall.posX, wall.posY)));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->up_boundary->size()) + " 'TOP' boundary leaves");
    for (Wall wall : *this->down_boundary) {
        this->root->D = insertNode(this->root->D, new QuadNode(Coords(wall.posX, wall.posY)));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->down_boundary->size()) + " 'DOWN' boundary leaves");
    for (Wall wall : *this->left_boundary) {
        this->root->L = insertNode(this->root->L, new QuadNode(Coords(wall.posX, wall.posY)));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->left_boundary->size()) + " 'LEFT' boundary leaves");
    for (Wall wall : *this->right_boundary) {
        this->root->R = insertNode(this->root->R, new QuadNode(Coords(wall.posX, wall.posY)));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->right_boundary->size()) + " 'RIGHT' boundary leaves");
};

void QSPTree::queryWalls(Coords origin, vector<Ray>* rays, vector<Coords>& ret_walls) {
    // TODO: implement tree traversal and node query against rays
    stack<QuadNode*> s;
    QuadNode* curr = this->root;
    int raysQueried = 0;
    while (raysQueried < rays->size() && (curr != nullptr || !s.empty())) {
        // NOTE: Add wall cordinates to ret_walls argument
        // NOTE: During traversal, cull branches that are not relevant
        // such as if the player is in the right branch and they are
        // looking through the right to the top then cull the left and bottom branches
        continue;
    }
};

void QSPTree::printPreorder(QuadNode* node) {
    if (node == nullptr) {
        return;
    }
    cout << node->wall_coords.asString();
    printPreorder(node->U);
    printPreorder(node->D);
    printPreorder(node->L);
    printPreorder(node->R);
};