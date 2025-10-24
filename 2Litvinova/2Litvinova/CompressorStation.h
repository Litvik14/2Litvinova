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
    int getId() const;
    const string& getName() const;
    int getNumberWork() const;
    int getNumberWorkOnline() const;
    const string& getClass() const;
    void setNumberWorkOnline(int n);
    float getIdlePercent() const;
    void print() const;
    void save(ofstream& out) const;
    void load(ifstream& in);
};