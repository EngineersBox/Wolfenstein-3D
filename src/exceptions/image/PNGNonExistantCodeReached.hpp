#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGNonExistantCodeReached : virtual public exception {
   protected:
    string message = "Unknown block reached";

   public:
    explicit PNGNonExistantCodeReached(){};

    virtual ~PNGNonExistantCodeReached() throw(){};

    virtual const char* what() const throw() {;
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            message);
        return strdup(message.c_str());
    };
};