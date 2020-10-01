#pragma once

#include <vector>

#include "../environment/Walls.hpp"
#include "../hashing/HashTable.hpp"

using namespace std;

struct BSPRegion{
    int y;
    int w;
    int h;
};

struct BSPNode {
    BSPNode* front;
    BSPNode* back;
    int depth;
    BSPRegion region;
};

class BSPTree {
    public:
        BSPTree();
        BSPTree(vector<Wall>* walls);
        ~BSPTree();

        void splitRec(BSPNode* node, int depth, int x_split);
        void getMinimumHulls();

    private:
        BSPNode* root;
        int iteration;
};