#pragma once

#include <exception>
#include <string>

using namespace std;

class FileFormatError : virtual public exception {
   protected:
    string format_ext;
    string message;
   public:
    explicit FileFormatError(const string& format):
        FileFormatError("Unrecognized file format: ", format)
    {};

    explicit FileFormatError(const string& msg, const string& format): 
        format_ext(format),
        message(msg)
    {};

    virtual ~FileFormatError() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + format_ext;
        return ret_val.c_str();
    };
};