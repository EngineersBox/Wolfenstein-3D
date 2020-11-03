#pragma once

#include <exception>
#include <string>

#include "../../rendering/Globals.hpp"

using namespace std;

class ExceededMaxTextureImport : virtual public exception {
    protected:
        int max_amount;

    public:
        explicit ExceededMaxTextureImport(int max_tex_amount):
            max_amount(max_tex_amount)
        {};

        virtual ~ExceededMaxTextureImport() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Maximum texture import count reached: " + to_string(max_amount);
            debugContext.glDebugMessageCallback(
                GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
                GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
                GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return strdup(ret_val.c_str());
        };
};