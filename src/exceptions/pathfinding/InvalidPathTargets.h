#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.h"

using namespace std;

class InvalidPathTargets : virtual public exception {
    protected:
        Coords start_pos;
        Coords end_pos;

    public:
        explicit InvalidPathTargets(Coords start, Coords end):
            start_pos(start),
            end_pos(end)
        {};

        virtual ~InvalidPathTargets() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Start and end targets are identical: (" + to_string(start_pos.first) + "," + to_string(start_pos.second) + ") == (" + to_string(start_pos.first) + "," + to_string(start_pos.second) + ")";
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};