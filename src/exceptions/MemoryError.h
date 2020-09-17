#pragma once

#include <exception>
#include <string>

using namespace std;

class MemoryError : virtual public exception {
   protected:
    string error_msg;
   public:
    explicit MemoryError(const string& msg):
        error_msg(msg)
    {};

    virtual ~MemoryError() throw(){};

    virtual const char* what() const throw() {
        return error_msg.c_str();
    };
};