#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGNoEndCode : virtual public exception {
   protected:
    string message = "No end code reached at length: ";
    size_t length;

   public:
    explicit PNGNoEndCode(size_t length):
        length(length)
    {};

    virtual ~PNGNoEndCode() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(length);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val
        );
        return strdup(ret_val.c_str());
    };
};