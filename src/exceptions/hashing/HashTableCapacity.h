#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.h"

using namespace std;

class HashTableCapacity : virtual public exception {
    protected:
        size_t max_size;
        string message;
    public:
     explicit HashTableCapacity(size_t max_capacity, const string& msg = "Hash table maximum size reached: ") :
        max_size(max_capacity),
        message(msg)
     {};

     virtual ~HashTableCapacity() throw(){};

     virtual const char* what() const throw() {
        string retVal = message;
        retVal.append(to_string(max_size));
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            retVal
        );
        return retVal.c_str();
    };
};