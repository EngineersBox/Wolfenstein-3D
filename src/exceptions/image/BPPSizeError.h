#pragma once

#include <exception>
#include <string>

using namespace std;

class BPPSizeError : virtual public exception {
    protected:
        uint16_t bit_count;

    public:
        explicit BPPSizeError(uint16_t bpp_count):
            bit_count(bpp_count)
        {};

        virtual ~BPPSizeError() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "BMP must use 24 or 32 bits per pixel: " + to_string(bit_count);
            return ret_val.c_str();
        };
};