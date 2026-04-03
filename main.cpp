#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <gmpxx.h>
#include <gmp.h>
#include <string>

using namespace std;

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

string hashCode(mpz_class arg) {

    size_t numOfDigits = mpz_sizeinbase(arg.get_mpz_t(), 10);
    string hash = "";

    while (numOfDigits--) {
        hash = HASH_VALUES[(arg.get_si() % 64)] + hash;
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

void print(mpz_class x, mpz_class arg, string func, string suffix, string symbol, auto startCalc) {

    cout << "[Done!]\nExporting... ";

    auto startExp = chrono::high_resolution_clock::now();
    string result = arg.get_str();
    string abr = "";

    if (result.length() >= 150) {
        abr = result.substr(0, 1) + "." + result.substr(1, 100) + "E+" + to_string(result.length());
    }

    auto endProc = chrono::high_resolution_clock::now();

    cout << "[Done!]\n\n";

    chrono::duration<double> calcTime_s = startExp - startCalc;
    chrono::duration<double> expTime_s = endProc - startExp;

    string calcTime = clockify(calcTime_s.count());
    string expTime = clockify(expTime_s.count());

    // if (result.length() < 150) {
    //     cout << x << symbol << " = " << result << endl;
    // }
    // else {
    //     cout << x << symbol << " = " << abr << endl;
    // }

    ofstream output(hashCode(x) + "-" + suffix + ".txt");

    if (!output.is_open()) {
        throw(runtime_error("could not open output file."));
    }

    output << "----- NO-LIMIT FACTORIAL REPORT -----\n" << endl;
    output << "Input: " << func << " of " << x << '\n' << '\n';

    output << "Calculations [==========][COMPLETED in " << calcTime << "]\n"
              "Exporting    [==========][COMPLETED in " << expTime << "]\n\n";

    output << "----- NO-LIMIT FACTORIAL RESULT -----\n" << endl;
    if (result.length() < 150) {
        output << "Short-form: " << result << endl;
    }
    else {
        output << "Short-form: " << abr << endl;
        output << "Long-form: \n" << result << endl;
    }

    output.close();
    cout << "\nResults got exported to " << hashCode(x) << "-" << suffix << ".txt" << endl;

    return;
}


void factorial(mpz_class x, string& calcTime, string& strgTime) {
    if (x < 0) {
        throw(runtime_error("factorial of negative."));
    }

    cout << "\nCalculating... ";

    auto startCalc = chrono::high_resolution_clock::now();

    if (x == 0) {
        print(0, 1, "factorial", "fact", "!", startCalc);
        return;
    }

    mpz_class result = 1;
    if (x > 0) {
        for (mpz_class i = 1; i <= x; i++) {
            result *= i;
        }
    }

    print(x, result, "factorial", "fact", "!", startCalc);
    return;
}

void double_factorial(mpz_class x, string& calcTime, string& strgTime) {
    if (x < 0) {
        throw(runtime_error("factorial of negative."));
    }

    cout << "\nCalculating... ";

    auto startCalc = chrono::high_resolution_clock::now();

    if (x == 0) {
        print(0, 1, "double factorial", "dfac", "!!", startCalc);
        return;
    }

    mpz_class result = 1;
    if (x > 0) {
        for (mpz_class i = x; i >= 1; i -= 2) {
            result *= i;
        }
    }

    print(x, result, "double factorial", "dfac", "!!", startCalc);
    return;
}

void termial(mpz_class x, string& calcTime, string& strgTime) {
    if (x < 0) {
        throw(runtime_error("termial of negative."));
    }

    cout << "\nCalculating... ";

    auto startCalc = chrono::high_resolution_clock::now();

    mpz_class result = (x * (x + 1)) / 2;

    print(x, result, "termial", "term", "?", startCalc);
    return;
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

    mpz_class x(value);
    string calcTime, strgTime;
    string result;

    switch (func_id) {
        case 0:
            factorial(x, calcTime, strgTime);
            break;

        case 1:
            double_factorial(x, calcTime, strgTime);
            break;

        case 2:
            termial(x, calcTime, strgTime);
            break;
    }

    cin.ignore();
    cin.get();

    return 0;
}