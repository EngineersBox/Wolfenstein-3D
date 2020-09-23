#pragma once

#include <exception>
#include <string>

using namespace std;

class ImagePixelError : virtual public exception {
    protected:
        u_int32_t x_pos, y_pos;
    public:
        explicit ImagePixelError(u_int32_t x, u_int32_t y):
            x_pos(x),
            y_pos(y)
        {};

        virtual ~ImagePixelError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Pixel location outside image boundaries: "
                + to_string(x_pos)
                + ", "
                + to_string(y_pos);
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_APPLICATION,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};