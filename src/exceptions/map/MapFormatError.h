#pragma once

#include <exception>
#include <string>

using namespace std;

class MapFormatError : virtual public exception {
    protected:
        int format;

    public:
        explicit MapFormatError(int format_val):
            format(format_val)
        {};

        virtual ~MapFormatError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Map format should be 0 or 1: " + to_string(format);
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_APPLICATION,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};