#pragma once

#include <exception>
#include <string>
#include <string.h>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGInvalidNLEN : virtual public exception {
   protected:
    string message = "NLEN is not one's complement of LEN (65535): NLEN = ";
    unsigned long nlen;
    unsigned long len;

   public:
    explicit PNGInvalidNLEN(unsigned long nlen, unsigned long len):
        nlen(len),
        len(len)
    {};

    virtual ~PNGInvalidNLEN() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(nlen) + " LEN = " + to_string(len);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};