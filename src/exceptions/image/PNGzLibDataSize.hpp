#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGzLibDataSize : virtual public exception {
   protected:
    string message = "zlib data size is too small: ";
    size_t data_size;

   public:
    explicit PNGzLibDataSize(size_t data_size) : data_size(data_size){};

    virtual ~PNGzLibDataSize() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(data_size);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val);
        return strdup(ret_val.c_str());
    };
};