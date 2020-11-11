#pragma once

#include <exception>
#include <string>
#include <string.h>

#include "../../rendering/Globals.hpp"

using namespace std;

class InvalidKeySize : virtual public exception {
    protected:
        string key;
    public:
     explicit InvalidKeySize(const string& key) :
        key(key)
     {};

     virtual ~InvalidKeySize() throw(){};

     virtual const char* what() const throw() {
        string retVal = "Key [" + key + "] is longer than 32 bytes: " + to_string(key.length());
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            retVal
        );
        return strdup(retVal.c_str());
    };
};