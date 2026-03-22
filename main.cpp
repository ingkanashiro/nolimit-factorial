#include <iostream>
#include <fstream>
#include <chrono>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;


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

int main() {

    cout << "----- NO-LIMIT FACTORIAL CALCULATOR -----\n"
            "Please, enter a number to calculate its factorial\n" << endl;

    cpp_int x;
    cin >> x;

    cpp_int factorial = 1;

    if (x < 0) {
        throw(runtime_error("factorial of negative."));
    }

    cout << "\nCalculating... ";

    auto startCalc = chrono::high_resolution_clock::now();

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

    auto startStr = chrono::high_resolution_clock::now();
    string str = factorial.str();

    cout << "[Done!]\n\n";

    auto finishProc = chrono::high_resolution_clock::now();

    chrono::duration<double> calcTime_s = startStr - startCalc;
    chrono::duration<double> strgTime_s = finishProc - startStr;

    string calcTime = clockify(calcTime_s.count());
    string strgTime = clockify(strgTime_s.count());

    if (str.length() < 150) {
        cout << x << "! = " << str << endl;

        cin.ignore();
        cin.get();

        return 0;
    }

    cout << x << "! = " << str[0] << "." << str.substr(1, 100) << "E+" << str.length() << endl;

    ofstream output(to_string(x) + "-factorial.txt");

    if (!output.is_open()) {
        throw(runtime_error("could not open output file."));
    }

    output << "----- NO-LIMIT FACTORIAL REPORT -----\n" << endl;
    output << "Input: " << x << '\n' << '\n';

    output << "Calculations [==========][COMPLETED in " << calcTime << "]\n"
              "Stringing    [==========][COMPLETED in " << strgTime << "]\n\n";

    output << "----- NO-LIMIT FACTORIAL RESULT -----\n" << endl;
    if (str.length() < 150) {
        output << "Short-form: " << str << endl;
    }
    else {
        output << "Short-form: " << str[0] << "." << str.substr(1,100) << "E+" << str.length() << endl;
        output << "Long-form: \n" << str << endl;
    }

    output.close();
    cout << "\nResults got exported to " << x << "-factorial.txt" << endl;

    cin.ignore();
    cin.get();

    return 0;
}