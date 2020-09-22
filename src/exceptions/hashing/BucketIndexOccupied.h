#pragma once

#include <exception>
#include <string>

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
            return ret_val.c_str();
        };
};