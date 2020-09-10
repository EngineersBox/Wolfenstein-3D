#include <utility>
#include <string>
#include <cstring>
#include <vector>

using namespace std;
using coords = pair<float, float>;

class Polygon {
    public:
        Polygon(const string& tex_ref, vector<coords> hull_points);
        ~Polygon();
        size_t hashPolygon();
    private:
        string convertHullPoints();
        size_t hashString(string string_to_hash);
        string _tex_ref;
        vector<coords> _hull_points;
};

Polygon::Polygon(const string& tex_ref, vector<coords> hull_points) {
    this->_tex_ref = tex_ref;
    this->_hull_points = hull_points;
}

Polygon::~Polygon() {
    delete &_tex_ref;
    delete &_hull_points;
}

string Polygon::convertHullPoints() {
    string return_val = "";
    for (coords point : this->_hull_points) {
        return_val.append(to_string(point.first));
        return_val.append(to_string(point.second));
    }
}

size_t Polygon::hashString(string string_to_hash){
    size_t hashed_string;
    // ... Do hash on string ...
    return hashed_string;
}

size_t Polygon::hashPolygon() {
    string total_polygon_string = this->_tex_ref;
    total_polygon_string.append(convertHullPoints());
    return hashString(total_polygon_string); // <-- Method to implement
}

Polygon example_polygon(
    "example_texture",
    vector<coords>{
        coords(2.1, -0.32),
        coords(0.36, 3.74),
        coords(-4.01, 5.21)
    }
);