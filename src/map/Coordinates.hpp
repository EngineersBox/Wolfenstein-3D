#pragma once

#include <string>
#include <iostream>

using namespace std;

template <typename T>
struct Coordinates {
    Coordinates(){};
    Coordinates(T x, T y) {
        this->x = x;
        this->y = y;
    };
    Coordinates(const Coordinates& other) {
        this->x = other.x;
        this->y = other.y;
    };

    bool isLeft(Coordinates other) {
        return other.x < this->x;
    }
    bool isAbove(Coordinates other) {
        return other.y < this->y;
    }

    bool operator==(const Coordinates& other) const {
        return this->x == other.x && this->y == other.y;
    }
    bool operator!=(const Coordinates& other) const {
        return this->x != other.x || this->y != other.y;
    }

    friend ostream& operator<<(ostream& os, const Coordinates& c) {
        return os << c.asString();
    }

    inline string asString() const {
        return "(" + to_string(this->x) + "," + to_string(this->y) + ")";
    }

    T x;
    T y;
};

typedef Coordinates<int> Coords;