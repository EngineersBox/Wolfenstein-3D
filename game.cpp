#include <vector>
#include <GLUT/glut.h>

using namespace std;
using coords = pair<float,float>;

// ---- BSP Tree ----

class Node {
    public:
        Node(vector<float> _hull_points);
        ~Node();
    private:
        vector<float> hull_points;
};

Node::Node(vector<float> _hull_points) {
    this->hull_points = _hull_points;
}

Node::~Node() {
    delete &hull_points;
}

class BSPTree {
    public:
        BSPTree();
        ~BSPTree();
    private:
        vector<Node> nodes;
};

BSPTree::BSPTree() {
    this->nodes = vector<Node>();
}

BSPTree::~BSPTree() {
    delete &nodes;
}

// ---- A* search ----

vector<coords> aStar() {

}

// ---- Main program ----

int main(int argc, char *argv[]) {
    return 0;
}