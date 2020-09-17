#pragma once

#include <exception>
#include <string>

using namespace std;

class FileFormatError : virtual public exception {
   protected:
    string format_ext;
   public:
    explicit FileFormatError(const string& format):
        format_ext(format)
    {};

    virtual ~FileFormatError() throw(){};

    virtual const char* what() const throw() {
        string ret_val = "Unrecognized file format: ";
        ret_val.append(format_ext);
        return ret_val.c_str();
    };
};