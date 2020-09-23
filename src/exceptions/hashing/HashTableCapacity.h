#pragma once

#include <exception>
#include <string>

using namespace std;

class HashTableCapacity : virtual public exception {
    protected:
        size_t max_size;
        string message;
    public:
     explicit HashTableCapacity(size_t max_capacity, const string& msg = "Hash table maximum size reached: ") :
        max_size(max_capacity),
        message(msg)
     {};

     virtual ~HashTableCapacity() throw(){};

     virtual const char* what() const throw() {
        string retVal = message;
        retVal.append(to_string(max_size));
        debugContext.glDebugMessageCallback(
            DEBUG_SOURCE_OS_X_SYSTEM,
            DEBUG_TYPE_ERROR,
            DEBUG_SEVERITY_HIGH,
            retVal
        );
        return retVal.c_str();
    };
};