#pragma once

#include <cmath>
#include <limits>
#include <stack>
#include <vector>

#include "../../environment/constructs/walls/AABB.hpp"
#include "../../environment/map/map.hpp"
#include "../../rendering/Globals.hpp"
#include "../../rendering/raycaster/Ray.hpp"

using namespace std;

struct QuadNode {
    QuadNode(Constructs::AABB& wall);
    ~QuadNode();

    Constructs::AABB wall;
    QuadNode* U = nullptr;
    QuadNode* D = nullptr;
    QuadNode* L = nullptr;
    QuadNode* R = nullptr;
    QuadNode* parent = nullptr;
};

QuadNode::QuadNode(Constructs::AABB& wall) {
    this->wall = wall;
}

QuadNode::~QuadNode() {
    delete U;
    delete D;
    delete L;
    delete R;
    delete parent;
}