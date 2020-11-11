#pragma once

#include <exception>
#include <string>
#include <string.h>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGInvalidFCHECK : virtual public exception {
   protected:
    string message = "256 * ";
    unsigned char zlib_d1;
    unsigned char zlib_d2;

   public:
    explicit PNGInvalidFCHECK(unsigned char zlib_d1, unsigned char zlib_d2):
        zlib_d1(zlib_d1),
        zlib_d2(zlib_d2)
    {};

    virtual ~PNGInvalidFCHECK() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(zlib_d1) + " + " + to_string(zlib_d2) + " must be a multiple of 31";
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};