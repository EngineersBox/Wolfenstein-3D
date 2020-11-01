#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class PNGInvalidDistCode : virtual public exception {
   protected:
    string message = "Invalid dist code specified, 30-31 are unused: ";
    unsigned long dist_code;

   public:
    explicit PNGInvalidDistCode(unsigned long dist_code): dist_code(dist_code) {};

    virtual ~PNGInvalidDistCode() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(dist_code);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};