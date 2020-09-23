#pragma once

#include <exception>
#include <string>

#include "../../raytracer/Globals.h"

using namespace std;

class ExceededDebugMessageSize : virtual public exception {
    protected:
        size_t msg_size;
        int max_msg_size;
    public:
        explicit ExceededDebugMessageSize(size_t message_size, int max_size):
            msg_size(message_size),
            max_msg_size(max_size)
        {};
        virtual ~ExceededDebugMessageSize() throw(){};

        virtual const char* what() const throw() {
            string msg = "GL debug message string is too long: ";
            msg += to_string(msg_size);
            msg += " > ";
            msg += to_string(max_msg_size);
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_APPLICATION,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_HIGH,
                msg
            );
            return msg.c_str();
        };
};