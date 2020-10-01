#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class BMPOriginPositionError : virtual public exception {
    protected:
        int32_t height_val;

    public:
        explicit BMPOriginPositionError(int32_t height):
            height_val(height)
        {};

        virtual ~BMPOriginPositionError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "BMP origin not in lower left corner: " + to_string(height_val);
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};