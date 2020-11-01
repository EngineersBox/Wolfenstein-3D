#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class PNGUnsupportedCompressionMethod : virtual public exception {
   protected:
    string message = "Only compression method 8 inflate with sliding window of 32k is supported: ";
    unsigned long cm;

   public:
    explicit PNGUnsupportedCompressionMethod(unsigned long cm):
        cm(cm)
    {};

    virtual ~PNGUnsupportedCompressionMethod() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(cm);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};