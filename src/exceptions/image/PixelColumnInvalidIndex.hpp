#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class PixelColumnInvalidIndex : virtual public exception {
    protected:
        int pixel_index;
    public:
        explicit PixelColumnInvalidIndex(int pixel_index):
            pixel_index(pixel_index)
        {};

        virtual ~PixelColumnInvalidIndex() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Pixel index is outside column boundary: "
                + to_string(pixel_index);
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};