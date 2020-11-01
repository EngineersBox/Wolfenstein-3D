#pragma once

#include <exception>
#include <string>

using namespace std;

class DebugLogWriteError : virtual public exception {
    protected:
        string fname;
    public:
        explicit DebugLogWriteError(const string& filename):
            fname(filename)
        {};
        virtual ~DebugLogWriteError() throw(){};

        virtual const char* what() const throw() {
            return strdup(string("Could not create new debug log: " + fname).c_str());
        };
};