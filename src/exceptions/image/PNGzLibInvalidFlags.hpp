#pragma once

#include <exception>
#include <string>
#include <string.h>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGzLibInvalidFlags : virtual public exception {
   protected:
    string message = "Additional flags should not specify a preset dictionary: ";
    unsigned long fdict;

   public:
    explicit PNGzLibInvalidFlags(unsigned long fdict):
        fdict(fdict)
    {};

    virtual ~PNGzLibInvalidFlags() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(fdict);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};