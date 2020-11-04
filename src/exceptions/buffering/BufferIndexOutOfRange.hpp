#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class BufferIndexOutOfRange : virtual public exception {
    protected:
        int index;
    public:
     explicit BufferIndexOutOfRange(int index):
        index(index)
     {};

     virtual ~BufferIndexOutOfRange() throw(){};

     virtual const char* what() const throw() {
         string retVal = "Index out of bounds of buffer: ";
         retVal.append(to_string(index));
         debugContext.glDebugMessageCallback(
             GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
             GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
             GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
             retVal);
         return strdup(retVal.c_str());
    };
};