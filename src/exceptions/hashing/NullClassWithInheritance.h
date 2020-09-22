#pragma once

#include <exception>
#include <string>

using namespace std;

class NullClassWithInheritance : virtual public exception {
    public:
        explicit NullClassWithInheritance(){};

        virtual ~NullClassWithInheritance() throw(){};

        virtual const char* what() const throw() {
            return "Supposed class with inheritance is a nullptr";
        };
};