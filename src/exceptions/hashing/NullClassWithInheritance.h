#pragma once

#include <exception>
#include <string>

using namespace std;

class NullClassWithInheritance : virtual public exception {
    public:
        explicit NullClassWithInheritance(){};

        virtual ~NullClassWithInheritance() throw(){};

        virtual const char* what() const throw() {
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_APPLICATION,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_HIGH,
                "Supposed class with inheritance is a nullptr"
            );
            return "Supposed class with inheritance is a nullptr";
        };
};