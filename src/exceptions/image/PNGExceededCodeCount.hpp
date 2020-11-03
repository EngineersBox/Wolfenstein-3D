#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGExceededCodeCount : virtual public exception {
   protected:
    string message = "Count exceeded total code count: ";
    size_t codecount;

   public:
    explicit PNGExceededCodeCount(size_t codecount) : codecount(codecount){};

    virtual ~PNGExceededCodeCount() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(codecount);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};