#pragma once

#include <exception>
#include <string>

using namespace std;

class INIReadError : virtual public exception {
    protected:
        string msg;

    public:
        explicit INIReadError(const string& message):
            msg(message)
        {};

        virtual ~INIReadError() throw(){};

        virtual const char* what() const throw() {
            return msg.c_str();
        };
};