#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;

class HashTableCapacity : virtual public exception {
    protected:
        size_t max_size;
        string message;
    public:
     explicit HashTableCapacity(size_t max_capacity):
        max_size(max_capacity),
        message("Hash table maximum size reached: ")
     {};
     explicit HashTableCapacity(size_t max_capacity, const string& msg) :
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