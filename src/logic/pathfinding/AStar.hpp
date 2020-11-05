#pragma once

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "../../environment/map/map.hpp"
#include "../../environment/map/Coordinates.hpp"
#include "../../exceptions/pathfinding/InvalidPathTargets.hpp"
#include "../../rendering/Globals.hpp"
#include "../../rendering/drawing/DrawingUtils.hpp"
#include "../../rendering/colour/Colours.hpp"
#include "GraphNode.hpp"
#include "../queue/MinHeap.hpp"

using namespace std;

class AStar {
    public:
        AStar();
        AStar(GameMap map);
        ~AStar();

        vector<Coords>* rebuildPath(unordered_map<GraphNode, GraphNode>& traversals, GraphNode start, GraphNode goal);
        vector<Coords>* find(Coords start_loc, Coords end_loc);
        void renderPath(vector<Coords>* path, Colour::ColorRGB path_colour, int sw, int sh, float scalingX, float scalingY);

       private:
        inline int heuristic(GraphNode next, GraphNode goal);
        inline bool inMap(GraphNode loc);
        vector<GraphNode> neighbors(GraphNode node);
        inline int gCost(GraphNode current, GraphNode next);
        inline void initCosts(unordered_map<GraphNode, int>& cost);
        void logPath(vector<Coords>& path);

        GameMap map;
};

AStar::AStar(){};

AStar::AStar(GameMap map) {
    this->map = map;
};

AStar::~AStar(){};

inline bool AStar::inMap(GraphNode node) {
    return node.x >= 0 && node.y >= 0 && node.x < map.map_width && node.y < map.map_height;
};

vector<GraphNode> AStar::neighbors(GraphNode node) {
    vector<GraphNode> map_neighbors;
    Coords loc_to_check[4][3] = {
        {Coords(-1,-1),Coords(0,-1),Coords(-1,0)},
        {Coords(1,-1),Coords(0,-1),Coords(1,0)},
        {Coords(1,1),Coords(1,0),Coords(0,1)},
        {Coords(-1,1),Coords(0,1),Coords(-1,0)}
    };
    for (int i = 0; i < 4; i++) {
        int cx{loc_to_check[i][0].x}, cy{loc_to_check[i][0].y};
        bool cInMap = inMap(Coords(node.x + cx, node.y + cy));
        bool cIsWall = !cInMap ? false : map.getAt(node.x + cx, node.y + cy).wf_left.texture != "";
        int cx1{loc_to_check[i][1].x}, cy1{loc_to_check[i][1].y};
        bool c1InMap = inMap(Coords(node.x + cx1, node.y + cy1));
        bool c1IsWall = !c1InMap ? false : map.getAt(node.x + cx1, node.y + cy1).wf_left.texture != "";
        int cx2{loc_to_check[i][1].x}, cy2{loc_to_check[i][1].y};
        bool c2InMap = inMap(Coords(node.x + cx2, node.y + cy2));
        bool c2IsWall = !c2InMap ? false : map.getAt(node.x + cx2, node.y + cy2).wf_left.texture != "";
        if (cInMap) {
            if (cIsWall) {
                if (c1InMap && !c1IsWall) {
                    map_neighbors.push_back(Coords(node.x + cx1, node.y + cy1));
                }
                if (c2InMap && ! c2IsWall) {
                    map_neighbors.push_back(Coords(node.x + cx2, node.y + cy2));
                }
                continue;
            }
            if (c1InMap && c1IsWall && c2InMap && c2IsWall) {
                continue;
            }
            map_neighbors.push_back(Coords(node.x + cx, node.y + cy));
        }
        if (c1InMap && !c1IsWall) {
            map_neighbors.push_back(Coords(
                node.x + cx1,
                node.y + cy1));
        }
        if (c2InMap && !c2IsWall) {
            map_neighbors.push_back(Coords(
                node.x + cx2,
                node.y + cy2));
        }
    }
    return map_neighbors;
};

vector<Coords>* AStar::rebuildPath(unordered_map<GraphNode, GraphNode>& traversals, GraphNode start, GraphNode goal) {
    vector<Coords> path;
    GraphNode current = goal;
    while (current != start) {
        path.push_back(Coords(current.x, current.y));
        current = GraphNode(traversals[current]);
    }
    path.push_back(Coords(start.x, start.y));
    reverse(path.begin(), path.end());
    logPath(path);
    return new vector<Coords>(path);
};

inline void AStar::initCosts(unordered_map<GraphNode, int>& cost) {
    for (int x = map.map_width - 1; x != -1; x--) {
        for (int y = map.map_height - 1; y != -1; y--) {
            cost.emplace(GraphNode(x, y), numeric_limits<int>::max());
        }
    }
};

inline int AStar::gCost(GraphNode current, GraphNode next) {
    return (current.x == next.x || current.y == next.y) ? 10 : 14;
}

inline int AStar::heuristic(GraphNode next, GraphNode goal) {
    return abs(next.x - goal.x) + abs(next.y - goal.y);
};

void AStar::logPath(vector<Coords>& path) {
    debugContext.logAppVerb(
        "Found path for " + map.start.asString() + " -> " + map.end.asString()
    );
    for_each(path.begin(), path.end(), [](Coords c) {
        debugContext.logAppVerb(c.asString());
    });
}

vector<Coords>* AStar::find(Coords start_loc, Coords end_loc) {
    if (start_loc == end_loc) {
        throw InvalidPathTargets(start_loc, end_loc);
    }
    GraphNode start(start_loc);
    GraphNode goal(end_loc);

    MinHeap<int, GraphNode> min_heap;
    min_heap.emplace(0, start);

    unordered_map<GraphNode, GraphNode> traversals;
    unordered_map<GraphNode, int> cost;
    initCosts(cost);

    traversals[start] = start;
    cost[start] = 0;

    while (!min_heap.isEmpty()) {
        GraphNode current = min_heap.top();
        min_heap.pop();

        if (current == goal) {
            break;
        }

        for (GraphNode next : neighbors(current)) {
            int g_cost = cost[current] + gCost(current, next);
            if (cost.find(next) == cost.end() || g_cost < cost[next]) {
                cost[next] = g_cost;
                int h_cost = heuristic(next, goal);
                int f_cost = g_cost + h_cost;
                next.g = g_cost;
                next.h = h_cost;
                next.f = f_cost;
                min_heap.emplace(f_cost, next);
                traversals[next] = current;
            }
        }
    }
    return rebuildPath(traversals, start, goal);
};

void AStar::renderPath(vector<Coords>* path, Colour::ColorRGB path_colour, int sw, int sh, float mapScalingX, float mapScalingY) {
    if (path->size() < 2) {
        return;
    }
    int xOffset = minimapCfg.isLeft() ? 0 : sw - minimapCfg.size;
    int yOffset = minimapCfg.isTop() ? 0 : sh - minimapCfg.size;
    int inBlockOffsetX = mapScalingX / 2.0;
    int inBlockOffsetY = mapScalingY / 2.0;

    int x, y;
    GLint current_colour[4];
    for (int i = 0; i < path->size() - 1; i++) {
        renderRay(
            xOffset + inBlockOffsetX + (path->at(i).x * mapScalingX),
            yOffset + inBlockOffsetY + (path->at(i).y * mapScalingX),
            xOffset + inBlockOffsetX + (path->at(i + 1).x * mapScalingX),
            yOffset + inBlockOffsetY + (path->at(i + 1).y * mapScalingX),
            2,
            path_colour
        );
    }
    glColor3i(
        current_colour[0],
        current_colour[1],
        current_colour[2]
    );
};