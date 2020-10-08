#pragma once

#include <algorithm>

#include "../raytracer/Globals.hpp"

using namespace std;

class GraphNode {
    public:
        GraphNode();
        GraphNode(int x, int y);
        GraphNode(Coords loc);

        inline bool operator==(const GraphNode& other) const;
        inline bool operator!=(const GraphNode& other) const;
        inline bool operator<(const GraphNode& other) const;

        int x;
        int y;
        int g = 0;
        int h = 0;
        int f = 0;
};

template <>
struct std::hash<GraphNode> {
    typedef GraphNode argument_type;
    typedef size_t result_type;
    size_t operator()(const GraphNode& key) const noexcept {
        // Using Cantor's enumeration of pairs to assign a
        // single natural number to every 2D coordinate pair
        return std::hash<int>()(((key.x + key.y) * (key.x + key.y + 1) / 2) + key.y);
    }
};

GraphNode::GraphNode(){};

GraphNode::GraphNode(Coords loc): GraphNode(loc.x, loc.y){};

GraphNode::GraphNode(int x, int y) {
    this->x = x;
    this->y = y;
}

inline bool GraphNode::operator==(const GraphNode& other) const {
    return (this->x == other.x
        &&  this->y == other.y);
}

inline bool GraphNode::operator!=(const GraphNode& other) const {
    return (this->x != other.x
        ||  this->y != other.y);
}

inline bool GraphNode::operator<(const GraphNode& other) const {
    return this->f < other.f;
}