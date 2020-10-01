#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.hpp"

using namespace std;
using ColourAttrib = string;
using ColourFormat = string;

class ImageColourError : virtual public exception {
    protected:
        ColourAttrib cAttrib;
        ColourFormat cFormat;
        string value_type;

    public:
        explicit ImageColourError(const ColourAttrib& attrib, const ColourFormat& format, const string& type):
            cAttrib(attrib),
            cFormat(format),
            value_type(type)
        {};

        virtual ~ImageColourError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Unexpected color "
                + cAttrib
                + "format! Expecting "
                + cFormat
                + " format: "
                + value_type;
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};