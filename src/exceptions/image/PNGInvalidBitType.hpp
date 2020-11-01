#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class PNGInvalidBitType : virtual public exception {
   protected:
    string message = "Invalid bit type: ";
    unsigned long bit_type;

   public:
    explicit PNGInvalidBitType(unsigned long bit_type):
        bit_type(bit_type)
    {};

    virtual ~PNGInvalidBitType() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(bit_type);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val
        );
        return strdup(ret_val.c_str());
    };
};