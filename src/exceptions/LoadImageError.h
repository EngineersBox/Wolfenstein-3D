#pragma once

#include <exception>
#include <string>

using namespace std;

enum ImageCause {
    WIDTH,
    HEIGHT,
    PLANE,
    BPP,
    FILEEXISTS,
    IDATA
};

string& operator <<(string& st, ImageCause ic) {
    switch (ic) {
        case WIDTH:
            st.append("width ");
            break;
        case HEIGHT:
            st.append("height ");
            break;
        case PLANE:
            st.append("planes ");
            break;
        case BPP:
            st.append("BPP ");
            break;
        case FILEEXISTS:
            st.append("non existant file ");
            break;
        case IDATA:
            st.append("image data ");
            break;
        default:
            st.append("INVALID ");
    }
    return st;
}

class LoadImageError: virtual public exception {
    protected:
        string error_msg = "Error on ";
        string filename_val;
        ImageCause cause;
        unsigned long l_data = (long) NULL;
        unsigned short s_data = (short) NULL;

        bool isComparator = false;
    public:
        explicit LoadImageError(const string& filename, ImageCause ic, unsigned long data):
            filename_val(filename),
            cause(ic),
            l_data(data)
        {};

        explicit LoadImageError(const string& filename, ImageCause ic, unsigned short data):
            filename_val(filename),
            cause(ic),
            s_data(data)
        {};

        explicit LoadImageError(const string& comparator, const string& filename, ImageCause ic, unsigned short data):
            error_msg(comparator),
            filename_val(filename),
            cause(ic),
            s_data(data),
            isComparator(true)
        {};

        virtual ~LoadImageError() throw() {};

        virtual const char* what() const throw() {
            if (isComparator) {
                string ret_msg  = "[";
                ret_msg.append(filename_val);
                ret_msg.append("] ");
                ret_msg << cause;
                ret_msg.append(error_msg);
                ret_msg.append(to_string(s_data));
                return ret_msg.c_str();
            }
            string ret_msg = "[";
            ret_msg.append(filename_val);
            ret_msg.append("] ");
            ret_msg.append(error_msg),
            ret_msg << cause;
            ret_msg.append(": ");
            ret_msg.append(to_string(l_data == (long) NULL ? s_data : l_data));
            return ret_msg.c_str();
        };
};