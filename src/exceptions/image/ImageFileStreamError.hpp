#pragma once

#include <exception>
#include <string>
#include <string.h>

#include "../../rendering/Globals.hpp"

using namespace std;

class ImageFileStreamError : virtual public exception {
    protected:
        string filename_val;

    public:
        explicit ImageFileStreamError(const string& filename):
            filename_val(filename)
        {};

        virtual ~ImageFileStreamError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Unable to open the input image file: " + filename_val;
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return strdup(ret_val.c_str());
        };
};