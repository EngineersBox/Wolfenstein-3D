#pragma once

#include <exception>
#include <string>
#include <string.h>

#include "../../rendering/Globals.hpp"

using namespace std;

class PNGOutsideCodeTree : virtual public exception {
   protected:
    string message = "Tree size outside bounds of ";
    int treepos;
    int bounds;

   public:
    explicit PNGOutsideCodeTree(int treepos, int bounds):
        treepos(treepos),
        bounds(bounds)
    {};

    virtual ~PNGOutsideCodeTree() throw(){};

    virtual const char* what() const throw() {
        string ret_val = message + to_string(bounds) + ": " + to_string(treepos);
        debugContext.glDebugMessageCallback(
            GL_DEBUG_SOURCE::DEBUG_SOURCE_APPLICATION,
            GL_DEBUG_TYPE::DEBUG_TYPE_ERROR,
            GL_DEBUG_SEVERITY::DEBUG_SEVERITY_HIGH,
            ret_val
        );
        return strdup(ret_val.c_str());
    };
};