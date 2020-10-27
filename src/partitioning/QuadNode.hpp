#pragma once

#include <cmath>
#include <limits>
#include <stack>
#include <vector>

#include "../environment/AABB.hpp"
#include "../map/map.hpp"
#include "../raytracer/Globals.hpp"
#include "../raytracer/Ray.hpp"

using namespace std;

struct QuadNode {
    QuadNode(AABB& wall);
    ~QuadNode();

    AABB wall;
    QuadNode* U = nullptr;
    QuadNode* D = nullptr;
    QuadNode* L = nullptr;
    QuadNode* R = nullptr;
    QuadNode* parent = nullptr;
};

QuadNode::QuadNode(AABB& wall) {
    this->wall = wall;
}

QuadNode::~QuadNode() {
    delete U;
    delete D;
    delete L;
    delete R;
    delete parent;
}