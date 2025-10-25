#pragma once
#include <string>
#include <fstream>

using namespace std;

class Pipe {
private:
    int id;
    string name;
    float length;
    int diametr;
    int status;

public:
    Pipe(int id = -1);
    void input();
    int getId() const { return id; }
    const string& getName() const { return name; }
    int getStatus() const { return status; }
    void setStatus(int s) { if (s == 0 || s == 1) status = s; }
    void print() const;
    void save(ofstream& out) const;
    void load(ifstream& in);
};