#pragma once
#include <string>
#include <fstream>

using namespace std;

class CompressorStation {
private:
    int id;
    string name;
    int number_work;
    int number_work_online;
    string class_cs;

public:
    CompressorStation(int id = -1);
    void input();
    int getId() const { return id; }
    const string& getName() const { return name; }
    float getIdlePercent() const {
        if (number_work == 0) return 0.0f;
        return (number_work - number_work_online) * 100.0f / number_work;
    }
    void setNumberWorkOnline(int n) {
        if (n >= 0 && n <= number_work) number_work_online = n;
    }
    void print() const;
    void save(ofstream& out) const;
    void load(ifstream& in);
};