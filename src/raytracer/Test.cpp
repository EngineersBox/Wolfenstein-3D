#include <string>
#include <iostream>

using namespace std;

int main() {
    string str = "ab";
    unsigned int octet = (str[0] << 4) + str[1];
    cout << octet << endl;
}