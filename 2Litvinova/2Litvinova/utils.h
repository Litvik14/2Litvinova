#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream> 
using namespace std;

string inputString(const string& prompt);
int inputInt(const string& prompt);
vector<int> parseIds(const string& line);
bool confirm(const string& msg);

template<typename T>
bool containsId(const unordered_map<int, T>& container, int id) {
    return container.find(id) != container.end();
}

template<typename T>
void printAllObjects(const unordered_map<int, T>& container) {
    for (const auto& item : container) {
        cout << item.second << "\n";
    }
}