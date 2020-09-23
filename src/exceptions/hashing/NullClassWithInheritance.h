#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.h"

using namespace std;

class NullClassWithInheritance : virtual public exception {
    public:
        explicit NullClassWithInheritance(){};

        virtual ~NullClassWithInheritance() throw(){};

        virtual const char* what() const throw() {
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                "Supposed class with inheritance is a nullptr"
            );
            return "Supposed class with inheritance is a nullptr";
        };
};