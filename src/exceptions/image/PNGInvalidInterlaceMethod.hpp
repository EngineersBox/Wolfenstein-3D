#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGInvalidInterlaceMethod : virtual public exception {
   protected:
    string message = "Interlace method must be 0 or 1, not: ";
    int interlace_method;

   public:
    explicit PNGInvalidInterlaceMethod(int interlace_method):
        interlace_method(interlace_method)
    {};

    virtual ~PNGInvalidInterlaceMethod() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(interlace_method);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val
        );
        return strdup(ret_val.c_str());
    };
};