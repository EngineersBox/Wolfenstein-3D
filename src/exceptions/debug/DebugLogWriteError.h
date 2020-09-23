#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.h"

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
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_OS_X_SYSTEM,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_HIGH,
                string("Could not create new debug log: " + fname)
            );
            return string("Could not create new debug log: " + fname).c_str();
        };
};