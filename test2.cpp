#include <string>
#include <vector>

using namespace std;
using coords = pair<float, float>;

struct Polygon {
    string text_ref;
    vector<coords> hull_points;
};

string convertHullPoints(vector<coords> &points) {
    string returnVal = "";
    for (coords point : points) {
        returnVal.append(to_string(point.first));
        returnVal.append(to_string(point.second));
    }
    return returnVal;
}

size_t hashString(string string_to_hash) {
    size_t hashed_string;
    // ... Do hash on string ...
    return hashed_string;
}

size_t hashPolygon(Polygon polygon) {
    string total_polygon_string = polygon.text_ref;
    total_polygon_string.append(convertHullPoints(polygon.hull_points));
    return hashString(total_polygon_string); // <-- Method to implement
}