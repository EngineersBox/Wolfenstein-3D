#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGInvalidFilterType : virtual public exception {
   protected:
    string message = "Unrecognized filter type: ";
    int filter;
   public:
    explicit PNGInvalidFilterType(int filter):
        filter(filter)
    {};

    virtual ~PNGInvalidFilterType() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(filter);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val
        );
        return strdup(ret_val.c_str());
    };
};