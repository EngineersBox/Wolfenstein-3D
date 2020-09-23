#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.h"

using namespace std;

class DebugLogDirCreateError : virtual public exception {
    public:
        explicit DebugLogDirCreateError(){};
        virtual ~DebugLogDirCreateError() throw(){};

        virtual const char* what() const throw() {
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_OS_X_SYSTEM,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_HIGH,
                "Could not create debug log directory"
            );
            return "Could not create debug log directory";
        };
};