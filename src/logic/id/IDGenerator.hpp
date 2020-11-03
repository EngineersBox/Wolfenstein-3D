#pragma once

#include "../../rendering/Globals.hpp"

using namespace std;

class IDGenerator {
    public:
        static IDGenerator* instance();
        int_id next();
    private:
        IDGenerator(): _id(0){};
        static IDGenerator* initial_static;
        int_id _id;
};

int_id IDGenerator::next(){
    return this->_id++;
};

IDGenerator* IDGenerator::initial_static{nullptr};

IDGenerator* IDGenerator::instance() {
    if (!initial_static) {
        initial_static = new IDGenerator();
    }
    return initial_static;
};