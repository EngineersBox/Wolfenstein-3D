#pragma once

#include <exception>
#include <string>

using namespace std;

class ImageDimensionsError : virtual public exception {
    protected:
        int32_t img_width, img_height;

    public:
        explicit ImageDimensionsError(int32_t width, int32_t height):
            img_width(width),
            img_height(height)
        {};

        virtual ~ImageDimensionsError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Image width and height must be positive numbers: "
                + to_string(img_width)
                + ", "
                + to_string(img_height);
            return ret_val.c_str();
        };
};