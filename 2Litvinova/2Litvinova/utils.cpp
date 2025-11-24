#include "utils.h"
#include <iostream>
#include <sstream>
#include <limits>
using namespace std;

string inputString(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    cerr << s << endl;
    return s;
}

int inputInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << x << endl;
            return x;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Try again.\n";
    }
}

vector<int> parseIds(const string& line) {
    vector<int> ids;
    stringstream ss(line);
    int id;
    while (ss >> id) ids.push_back(id);
    return ids;
}

bool confirm(const string& msg) {
    cout << msg << " (y/n): ";
    char c;
    cin >> c;
    cin.ignore();
    cerr << c << endl;
    return (c == 'y' || c == 'Y');
}