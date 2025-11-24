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
    void set(const string& n, int nw, int on, const string& c) {
        name = n; number_work = nw; number_work_online = on; class_cs = c;
    }
    int getId() const { return id; }
    const string& getName() const { return name; }
    int getNumberWork() const { return number_work; }
    int getNumberWorkOnline() const { return number_work_online; }
    const string& getClass() const { return class_cs; }
    float getIdlePercent() const {
        if (number_work == 0) return 0.0f;
        return (number_work - number_work_online) * 100.0f / number_work;
    }
    void setNumberWorkOnline(int n) {
        if (n >= 0 && n <= number_work) number_work_online = n;
    }

    bool operator==(const CompressorStation& other) const {
        return id == other.id;
    }
    friend ostream& operator<<(ostream& os, const CompressorStation& cs);

    void print() const;
    void save(ofstream& out) const;
    void load(ifstream& in);
};