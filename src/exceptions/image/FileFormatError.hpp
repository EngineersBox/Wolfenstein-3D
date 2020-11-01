#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class FileFormatError : virtual public exception {
   protected:
    string format_ext;
    string message;
   public:
    explicit FileFormatError(const string& format):
        FileFormatError("Unrecognized file format: ", format)
    {};

    explicit FileFormatError(const string& msg, const string& format): 
        format_ext(format),
        message(msg)
    {};

    virtual ~FileFormatError() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + format_ext;
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val
        );
        return strdup(ret_val.c_str());
    };
};