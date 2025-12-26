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
    void set(const string& n, float l, int d, int s) {
        name = n; length = l; diametr = d; status = s;
    }
    int getId() const { return id; }
    const string& getName() const { return name; }
    float getLength() const { return length; }
    int getDiametr() const { return diametr; }
    int getStatus() const { return status; }
    void setStatus(int s) { if (s == 0 || s == 1) status = s; }

    bool operator==(const Pipe& other) const {
        return id == other.id;
    }
    friend ostream& operator<<(ostream& os, const Pipe& p);

    void print() const;
    void save(ofstream& out) const;
    void load(ifstream& in);
};