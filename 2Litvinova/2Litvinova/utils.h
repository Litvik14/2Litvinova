#pragma once
#include <string>
#include <vector>

using namespace std;

string inputString(const string& prompt);
int inputInt(const string& prompt, int min = -1, int max = -1);
float inputFloat(const string& prompt, float min = 0.0f);
vector<int> selectIdsFromList(const vector<int>& candidates);

void logAction(const string& message);
void setLogFile(const string& filename);