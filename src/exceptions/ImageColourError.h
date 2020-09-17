#pragma once

#include <exception>
#include <string>

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
            string ret_val = "Unexpected color ";
            ret_val.append(cAttrib);
            ret_val.append("format! Expecting ");
            ret_val.append(cFormat);
            ret_val.append(" format: ");
            ret_val.append(value_type);
            return ret_val.c_str();
        };
};