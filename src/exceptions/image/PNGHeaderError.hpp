#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGHeaderError : virtual public exception {
   protected:
    string message;
    size_t size_val;

   public:
    explicit PNGHeaderError(string message, size_t size_val):
        message(message),
        size_val(size_val)
    {};

    virtual ~PNGHeaderError() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(size_val);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};