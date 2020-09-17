#pragma once

#include <exception>
#include <string>

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
            string ret_val = "Unable to open the input image file: ";
            ret_val.append(filename_val);
            return ret_val.c_str();
        };
};