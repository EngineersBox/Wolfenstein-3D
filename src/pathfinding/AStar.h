#pragma once

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

#include "../map/map.h"
#include "../raytracer/Globals.h"

using namespace std;

class GraphNode {
    public:
        GraphNode();
        GraphNode(int x, int y);
        GraphNode(Coords loc);

        inline bool operator==(const GraphNode& other);
        inline bool operator!=(const GraphNode& other);

        int x;
        int y;
        float g = 0;
        float h = 0;
        float f = 0;
};

template <> struct hash<GraphNode> {
  typedef GraphNode argument_type;
  typedef size_t result_type;
  size_t operator()(const GraphNode& id) const noexcept {
    return hash<int>()(id.x ^ (id.y << 4));
  }
};

GraphNode::GraphNode(){};

GraphNode::GraphNode(Coords loc): GraphNode(loc.first, loc.second){};

GraphNode::GraphNode(int x, int y) {
    this->x = x;
    this->y = y;
}

GraphNode::~GraphNode(){};

inline bool GraphNode::operator==(const GraphNode& other) {
    return (this->x == other.x
        &&  this->y == other.y);
}

inline bool GraphNode::operator==(const GraphNode& other) {
    return (this->x != other.x
        &&  this->y != other.y);
}

class AStar {
    public:
        AStar();
        ~AStar();

        double heuristic(GraphNode next, GraphNode goal);
        bool inMap(GraphNode loc);
        vector<GraphNode> neighbors(GraphNode node);
        vector<GraphNode>* reconstructPath(unordered_map<GraphNode, GraphNode> came_from, GraphNode start, GraphNode goal);
        vector<GraphNode>* find(Coords start_loc, Coords end_loc);

        GameMap map;
};

template <typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<
        PQElement,
        std::vector<PQElement>,
        std::greater<PQElement>
    > elements;

    inline bool empty() const {
        return elements.empty();
    }

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

bool AStar::inMap(GraphNode node) {
    return node.x >= 0 && node.y >= 0 && node.x < map.map_width && node.y < map.map_height;
};

vector<GraphNode> AStar::neighbors(GraphNode node) {
    vector<GraphNode> map_neighbors;
    for (int x_off = -1; x_off <= 1; x_off++) {
        for (int y_off = -1; y_off <= 1; y_off++) {
            if (x_off == 0 && y_off == 0) {
                continue;
            }
            if (inMap(Coords{node.x + x_off, node.y + y_off})) {
                map_neighbors.push_back(Coords{node.x + x_off, node.y + y_off});
            }
        }
    }
    return map_neighbors;
};

double AStar::heuristic(GraphNode next, GraphNode goal) {
    return abs(next.x - goal.x) + abs(next.y - goal.y);
};

vector<GraphNode>* AStar::reconstructPath(unordered_map<GraphNode, GraphNode> came_from, GraphNode start, GraphNode goal) {
    vector<GraphNode> path;
    GraphNode current = goal;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);  // optional
    reverse(path.begin(), path.end());
    return new vector<GraphNode>(path);
}

vector<GraphNode>* AStar::find(Coords start_loc, Coords end_loc) {
    GraphNode start(start_loc);
    GraphNode goal(end_loc);

    PriorityQueue<GraphNode, double> head;
    head.put(start, 0);

    unordered_map<GraphNode, GraphNode> came_from;
    unordered_map<GraphNode, double> cost;

    came_from[start] = start;
    cost[start] = 0;

    while (!head.empty()) {
        GraphNode current = head.get();

        if (current == goal) {
            break;
        }

        for (GraphNode next : neighbors(current)) {
            double g_cost = cost[current] + 1;
            if (cost.find(next) == cost.end() || g_cost < cost[next]) {
                cost[next] = g_cost;
                double f_cost = g_cost + heuristic(next, goal);
                head.put(next, f_cost);
                came_from[next] = current;
            }
        }
    }
    return reconstructPath(came_from, start, goal);
}