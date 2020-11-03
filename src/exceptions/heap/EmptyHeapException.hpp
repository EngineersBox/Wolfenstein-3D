#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class EmptyHeapException : virtual public exception {
   protected:
    int heap_size;
    int idx;

   public:
    explicit EmptyHeapException(){};

    virtual ~EmptyHeapException() throw(){};

    virtual const char* what() const throw() {
        string ret_val = "Min heap is of size 0";
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};