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
    int getId() const;
    const string& getName() const;
    float getLength() const;
    int getDiametr() const;
    int getStatus() const;
    void setStatus(int s);
    void print() const;
    void save(ofstream& out) const;
    void load(ifstream& in);
};