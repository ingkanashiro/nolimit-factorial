#include <iostream>
#include <fstream>
#include <chrono>
#include <boost/math/ccmath/abs.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

const string FUNCTION_LIST_LANG[] = {
    "factorial",
    "double factorial",
    "termial"
};
const string FUNCTION_FILE_SUFFIX[] = {
    "fact",
    "dfac",
    "term"
};

const char HASH_VALUES[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '-', '_'
};

string hashCode(cpp_int arg) {

    cpp_int numOfDigits = (msb(arg) / 6) + 1;
    string hash = "";

    while (numOfDigits--) {
        hash = HASH_VALUES[(int)(arg % 64)] + hash;
        arg /= 64;
    }

    return hash;
}

string clockify(double time) {

    int d = 0, h = 0, m = 0, s = 0, ms = 0;

    // check for days
    if (time >= 86400.0) {
        d = floor(time / 86400.0);
        time -= d * 86400.0;
    }

    // check for hours
    if (time >= 3600.0) {
        h = floor(time / 3600.0);
        time -= h * 3600.0;
    }

    // check for minutes
    if (time >= 60.0) {
        m = floor(time / 60.0);
        time -= m * 60.0;
    }

    // check for seconds
    if (time >= 1) {
        s = floor(time);
        time -= s;
    }

    // miliseconds
    time *= 1000;
    ms = ceil(time);


    // Format 1d::15h:20m:05s.267ms
    string clockTime = "";
    string strings[] = {
        to_string(d),
        to_string(h),
        to_string(m),
        to_string(s),
        to_string(ms)
    };
    int len[] = {
        1,
        2,
        2,
        2,
        3
    };
    string prefix[] = {
        "d",
        "h",
        "m",
        "s",
        "ms"
    };

    int firstIndex = 4;

    if (s > 0) {
        firstIndex = 3;
    }
    if (m > 0) {
        firstIndex = 2;
    }
    if (h > 0) {
        firstIndex = 1;
    }
    if (d > 0) {
        firstIndex = 0;
    }

    for (int i = 0; i < 5; i++) {

        if (i < firstIndex) {
            strings[i] = "";
        }
        else {
            string str = strings[i];

            if (i > firstIndex) {
                // Add trailing zeros
                while (str.length() < len[i]) {
                    str = "0" + str;
                }
            }

            str += prefix[i];

            if (i == 0) {
                str += "::";
            }
            else if (i == 1 || i == 2) { // 0d::01h:02m:03s.004ms
                str += ":";
            }
            else if (i == 3) {
                str += ".";
            }

            strings[i] = str;
        }
    }

    string result = "";
    for (string str : strings) {
        result += str;
    }

    return result;
}

string stringify(cpp_int arg, string& calcTime, string& strgTime, auto startCalc) {
    cout << "[Done!]\nStringing... ";

    auto startStr = chrono::high_resolution_clock::now();
    string str = arg.str();

    cout << "[Done!]\n\n";

    auto finishProc = chrono::high_resolution_clock::now();

    chrono::duration<double> calcTime_s = startStr - startCalc;
    chrono::duration<double> strgTime_s = finishProc - startStr;

    calcTime = clockify(calcTime_s.count());
    strgTime = clockify(strgTime_s.count());

    return str;
}

string factorial(cpp_int x, string& calcTime, string& strgTime) {
    if (x < 0) {
        throw(runtime_error("factorial of negative."));
    }

    cout << "\nCalculating... ";

    auto startCalc = chrono::high_resolution_clock::now();

    if (x == 0) {
        return "1";
    }

    cpp_int result = 1;
    if (x > 0) {
        for (cpp_int i = 1; i <= x; i++) {
            result *= i;
        }
    }

    string str = stringify(result, calcTime, strgTime, startCalc);

    return str;
}

string double_factorial(cpp_int x, string& calcTime, string& strgTime) {
    if (x < 0) {
        throw(runtime_error("factorial of negative."));
    }

    cout << "\nCalculating... ";

    auto startCalc = chrono::high_resolution_clock::now();

    if (x == 0) {
        return "1";
    }

    cpp_int result = 1;
    if (x > 0) {
        for (cpp_int i = x; i >= 1; i -= 2) {
            result *= i;
        }
    }

    string str = stringify(result, calcTime, strgTime, startCalc);

    return str;
}

string termial(cpp_int x, string& calcTime, string& strgTime) {
    if (x < 0) {
        throw(runtime_error("termial of negative."));
    }

    cout << "\nCalculating... ";

    auto startCalc = chrono::high_resolution_clock::now();

    cpp_int result = (x * (x + 1)) / 2;
    string str = stringify(result, calcTime, strgTime, startCalc);

    return str;
}

void print(string arg, cpp_int x, string func, int func_id, string calcTime, string strgTime) {

    if (arg.length() < 150) {
        cout << x << func << " = " << arg << endl;
    }
    else {
        cout << x << func << " = " << arg[0] << "." << arg.substr(1, 100) << "E+" << arg.length() << endl;
    }

    ofstream output(hashCode(x) + "-" + FUNCTION_FILE_SUFFIX[func_id] + ".txt");

    if (!output.is_open()) {
        throw(runtime_error("could not open output file."));
    }

    output << "----- NO-LIMIT FACTORIAL REPORT -----\n" << endl;
    output << "Input: " << FUNCTION_LIST_LANG[func_id] << " of " << x << '\n' << '\n';

    output << "Calculations [==========][COMPLETED in " << calcTime << "]\n"
              "Stringing    [==========][COMPLETED in " << strgTime << "]\n\n";

    output << "----- NO-LIMIT FACTORIAL RESULT -----\n" << endl;
    if (arg.length() < 150) {
        output << "Short-form: " << arg << endl;
    }
    else {
        output << "Short-form: " << arg[0] << "." << arg.substr(1,100) << "E+" << arg.length() << endl;
        output << "Long-form: \n" << arg << endl;
    }

    output.close();
    cout << "\nResults got exported to " << hashCode(x) << "-" << FUNCTION_FILE_SUFFIX[func_id] << ".txt" << endl;

}

int main() {
    cout << "----- NO-LIMIT FACTORIAL CALCULATOR -----\n"
            "Please, enter a computation (e.g.: 10!, 500!!, 365?, ...)\n" << endl;

    string input;
    cin >> input;

    // Get the function to apply by splitting the input
    int funcPtr = -1;
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '!' || input[i] == '?') {
            funcPtr = i;
            break;
        }
    }

    int func_id;

    /*  FUNCTION ID LIST:
     *
     *  0 -> factorial (!)
     *  1 -> double factorial (!!)
     *  2 -> termial (?)
     *  3 -> ...
     *
     */

    string value, func;

    if (funcPtr == -1) {    // Assume factorial function by default
        func_id = 0;
        value = input;
    }
    else {

        value = input.substr(0, funcPtr);
        func = input.substr(funcPtr, input.length() - 1);

        switch (func[0]) {
            case '!':

                if (func.length() == 2) {
                    func_id = 1;
                }
                else if (func.length() == 1) {
                    func_id = 0;
                }
                else {
                    throw runtime_error("could not resolve symbol " + func + " at input");
                }

                break;

            case '?':
                func_id = 2;
                break;

            default:
                throw runtime_error("could not resolve symbol " + func + " at input");
        }
    }

    cpp_int x(value);
    string calcTime, strgTime;
    string result;

    switch (func_id) {
        case 0:
            result = factorial(x, calcTime, strgTime);
            break;

        case 1:
            result = double_factorial(x, calcTime, strgTime);
            break;

        case 2:
            result = termial(x, calcTime, strgTime);
            break;
    }

    print(result, x, func, func_id, calcTime, strgTime);

    cin.ignore();
    cin.get();

    return 0;
}