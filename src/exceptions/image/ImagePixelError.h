#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.h"

using namespace std;

class ImagePixelError : virtual public exception {
    protected:
        u_int32_t x_pos, y_pos;
    public:
        explicit ImagePixelError(u_int32_t x, u_int32_t y):
            x_pos(x),
            y_pos(y)
        {};

        virtual ~ImagePixelError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Pixel location outside image boundaries: "
                + to_string(x_pos)
                + ", "
                + to_string(y_pos);
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};