#pragma once

#include <exception>
#include <string>
#include "../../raytracer/Globals.h"

using namespace std;

class BucketIndexOccupied : virtual public exception {
    protected:
        int idx;
    public:
        explicit BucketIndexOccupied(int hash_idx):
            idx(hash_idx)
        {};

        virtual ~BucketIndexOccupied() throw(){};

        virtual const char* what() const throw() {
            string ret_val = "Element already exists at bucket index: ";
            ret_val.append(to_string(idx));
            debugContext.glDebugMessageCallback(
                DEBUG_SOURCE_APPLICATION,
                DEBUG_TYPE_ERROR,
                DEBUG_SEVERITY_HIGH,
                ret_val
            );
            return ret_val.c_str();
        };
};