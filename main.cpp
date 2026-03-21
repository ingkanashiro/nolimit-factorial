#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

int main() {

    const int DIGITS_TO_SHOW = 50;

    cpp_int x;
    cin >> x;

    cpp_int factorial = 1;

    if (x < 0) {
        throw(runtime_error("factorial of negative."));
    }

    cout << "Calculating... ";
    if (x == 0) {
        cout << "0! = 1" << endl;
        return 0;
    }

    if (x > 0) {
        for (cpp_int i = 1; i <= x; i++) {
            factorial *= i;
        }
    }

    cout << "[Done!]\nStringing... ";
    string str = factorial.str();

    cout << "[Done!]" << endl << endl;

    if (str.length() < 150) {
        cout << x << "! = " << str << endl;
        return 0;
    }

    cout << x << "! = " << str[0] << "." << str.substr(1, 100) << "E+" << str.length() << endl;

    string input;
    cin >> input;

    if (input == "expand") {
        cout << str << endl;
    }

    cin.ignore();
    cin.get();

    return 0;
}