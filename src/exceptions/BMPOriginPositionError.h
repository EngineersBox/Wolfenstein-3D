#pragma once

#include <exception>
#include <string>

using namespace std;

class BMPOriginPositionError : virtual public exception {
    protected:
        int32_t height_val;

    public:
        explicit BMPOriginPositionError(int32_t height):
            height_val(height)
        {};

        virtual ~BMPOriginPositionError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "BMP origin not in lower left corner: " + to_string(height_val);
            return ret_val.c_str();
        };
};