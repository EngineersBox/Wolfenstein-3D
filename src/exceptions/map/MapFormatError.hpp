#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class MapFormatError : virtual public exception {
    protected:
        int format;

    public:
        explicit MapFormatError(int format_val):
            format(format_val)
        {};

        virtual ~MapFormatError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Map format should be 0 or 1: " + to_string(format);
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return strdup(ret_val.c_str());
        };
};