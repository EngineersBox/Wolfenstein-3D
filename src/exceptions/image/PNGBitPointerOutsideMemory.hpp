#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class PNGBitPointerOutsideMemory : virtual public exception {
   protected:
    string message = "Bit pointer references unavailable memory: ";
    size_t pointer;

   public:
    explicit PNGBitPointerOutsideMemory(size_t pointer):
        pointer(pointer)
    {};

    virtual ~PNGBitPointerOutsideMemory() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + toHex(pointer);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val
        );
        return strdup(ret_val.c_str());
    };
};