#pragma once

#include <cmath>
#include <stack>
#include <vector>

#include "../environment/AABB.hpp"
#include "../map/Coordinates.hpp"
#include "../map/map.hpp"
#include "../raytracer/Globals.hpp"
#include "../raytracer/Ray.hpp"
#include "QuadNode.hpp"

using namespace std;

enum RelativePosition : size_t {
    REL_UP = 0,
    REL_DOWN = 1,
    REL_LEFT = 2,
    REL_RIGHT = 3
};

struct TraversalRecord {
    TraversalRecord(QuadNode* node, vector<RelativePosition> vec) {
        this->node = node;
        this->traversals = vec;
    };
    QuadNode* node;
    vector<RelativePosition> traversals;
};

class QSPTree {
    public:
        QSPTree();
        QSPTree(GameMap map, vector<AABB>* ordered_walls, vector<AABB>* u_bound, vector<AABB>* d_bound, vector<AABB>* l_bound, vector<AABB>* r_bound);
        ~QSPTree();

        void buildTree();
        QuadNode* insertNode(QuadNode* root, QuadNode* node);
        inline QuadNode* insertRelative(QuadNode* dir_node, QuadNode* node);

        void queryWalls(Coords origin, float player_angle, vector<Ray>& rays, vector<Coords>& ret_walls);

        void printPreorder(QuadNode* node);
    private:
        RelativePosition position(Coords a, Coords b);
        double sqDist(Coords a, Coords b);
        void findMiddle();
        inline void traverseNext(RelativePosition& currBranch, QuadNode* currNode, TraversalRecord& currTraversal, stack<TraversalRecord>& nodesVisited);

        GameMap map;
        vector<AABB>* walls;
        vector<AABB>* up_boundary;
        vector<AABB>* down_boundary;
        vector<AABB>* left_boundary;
        vector<AABB>* right_boundary;
        AABB mid;
        QuadNode* root;
};

QSPTree::QSPTree(){};

QSPTree::QSPTree(GameMap map, vector<AABB>* ordered_walls, vector<AABB>* u_bound, vector<AABB>* d_bound, vector<AABB>* l_bound, vector<AABB>* r_bound) {
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
    AABB current_mid;
    Coords cMid = Coords((int)floor(map.map_width), (int)floor(map.map_height));
    double current_min_dist = numeric_limits<double>::max();
    int current_wall_idx;
    for (int i = walls->size() - 1; i != -1; i--) {
        Coords loc = Coords(walls->at(i).location.x, walls->at(i).location.y);
        int dist = sqDist(cMid, loc);
        if (dist < current_min_dist) {
            current_mid = walls->at(i);
            current_min_dist = dist;
            current_wall_idx = i;
        }
    }
    this->walls->erase(this->walls->begin() + current_wall_idx);
    this->mid = current_mid;
    debugContext.logAppInfo("Closest AABB to actuall map centre at: (" + to_string(current_mid.location.x) + "," + to_string(current_mid.location.y) + ")");
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
    RelativePosition relPos = position(Coords(root->wall.location.x, root->wall.location.y), Coords(node->wall.location.x, node->wall.location.y));
    switch (relPos) {
        case RelativePosition::REL_UP:
            root->U = insertRelative(root->U, node);
            break;
        case RelativePosition::REL_DOWN:
            root->D = insertRelative(root->D, node);
            break;
        case RelativePosition::REL_LEFT:
            root->L = insertRelative(root->L, node);
        default:
            root->R = insertRelative(root->R, node);
            break;
    }
    return root;
};

void QSPTree::buildTree() {
    debugContext.logAppInfo("---- STARTED BUILDING QSP TREE ----");
    for (AABB AABB : *this->walls) {
        this->root = insertNode(this->root, new QuadNode(AABB));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->walls->size()) + " AABB nodes");
    // Inserting boundaries last will garuantee them as leaf nodes
    for (AABB AABB : *this->up_boundary) {
        this->root->U = insertNode(this->root->U, new QuadNode(AABB));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->up_boundary->size()) + " 'TOP' boundary leaves");
    for (AABB AABB : *this->down_boundary) {
        this->root->D = insertNode(this->root->D, new QuadNode(AABB));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->down_boundary->size()) + " 'DOWN' boundary leaves");
    for (AABB AABB : *this->left_boundary) {
        this->root->L = insertNode(this->root->L, new QuadNode(AABB));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->left_boundary->size()) + " 'LEFT' boundary leaves");
    for (AABB AABB : *this->right_boundary) {
        this->root->R = insertNode(this->root->R, new QuadNode(AABB));
    }
    debugContext.logAppInfo("Inserted " + to_string(this->right_boundary->size()) + " 'RIGHT' boundary leaves");
    debugContext.logAppInfo("---- FINISHED BUILDING QSP TREE [" + string(ADDR_OF(*this)) + "] ----");
};

inline void QSPTree::traverseNext(RelativePosition& currBranch, QuadNode* currNode, TraversalRecord& currTraversal, stack<TraversalRecord>& nodesVisited) {
    switch (currBranch) {
        case RelativePosition::REL_UP:
            currNode = currNode->U;
            currTraversal.traversals.push_back(RelativePosition::REL_UP);
            nodesVisited.push(currTraversal);
            break;
        case RelativePosition::REL_LEFT:
            currNode = currNode->L;
            currTraversal.traversals.push_back(RelativePosition::REL_LEFT);
            nodesVisited.push(currTraversal);
            break;
        case RelativePosition::REL_DOWN:
            currNode = currNode->D;
            currTraversal.traversals.push_back(RelativePosition::REL_DOWN);
            nodesVisited.push(currTraversal);
            break;
        default:
            currNode = currNode->R;
            currTraversal.traversals.push_back(RelativePosition::REL_RIGHT);
            nodesVisited.push(currTraversal);
            break;
    }
};

void QSPTree::queryWalls(Coords origin, float player_angle, vector<Ray>& rays, vector<Coords>& ret_walls) {
    // TODO: implement tree traversal and node query against rays
    stack<TraversalRecord> nodesVisited;
    TraversalRecord currTraversal(this->root, vector<RelativePosition>());
    QuadNode* currNode = this->root;
    Coords cPos = Coords(radToCoord(currNode->wall.location.x) + 0.5, radToCoord(currNode->wall.location.y) + 0.5);
    RelativePosition currBranch = position(cPos, origin);
    int raysQueried = 0;
    debugContext.logAppVerb("Querying " + to_string(rays.size()) + " rays in QSP tree [" + ADDR_OF(*this) + "]");
    while (raysQueried < rays.size() && (!nodesVisited.empty())) {
        if (currNode == nullptr) {
            currTraversal = nodesVisited.top();
            nodesVisited.pop();
            currNode = currTraversal.node;
            continue;
        }

        cPos = Coords(radToCoord(currNode->wall.location.x) + 0.5, radToCoord(currNode->wall.location.y) + 0.5);
        currBranch = position(cPos, origin);

        vector<Ray> newRays;
        for (Ray ray : rays) {
            Coords intersection;
            if (currNode->wall.intersect(ray, intersection)) {
                raysQueried++;
                ret_walls.push_back(intersection);
            } else {
                newRays.push_back(ray);
            }
        }
        rays = newRays;

        RelativePosition nextBranch = currBranch;
        if (currTraversal.traversals.size() == 4) {
            currNode = nodesVisited.top().node;
            nodesVisited.pop();
            continue;
        }
        // 3.1 If already searched branch containing ray origin, try other branches
        if (find(currTraversal.traversals.begin(), currTraversal.traversals.end(), currBranch) == currTraversal.traversals.end()) {
            // 3.2 If already searched branch that rays at facing towards try other branches
            // 3.3 If already searched non-behind branch, try behind branch
        }
        traverseNext(currBranch, currNode, currTraversal, nodesVisited);
    }
};

void QSPTree::printPreorder(QuadNode* node) {
    if (node == nullptr) {
        return;
    }
    cout << "(" + to_string(node->wall.location.x) + "," + to_string(node->wall.location.y) + ")";
    printPreorder(node->U);
    printPreorder(node->D);
    printPreorder(node->L);
    printPreorder(node->R);
};