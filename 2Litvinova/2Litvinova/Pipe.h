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
    int from_cs;
    int to_cs;

public:
    Pipe(int id = -1);
    void input();
    void set(const string& n, float l, int d, int s, int from = -1, int to = -1) {
        name = n; length = l; diametr = d; status = s;
        from_cs = from; to_cs = to;
    }
    int getId() const { return id; }
    const string& getName() const { return name; }
    float getLength() const { return length; }
    int getDiametr() const { return diametr; }
    int getStatus() const { return status; }
    int getFromCS() const { return from_cs; }
    int getToCS() const { return to_cs; }
    void setStatus(int s) { if (s == 0 || s == 1) status = s; }
    bool isConnected() const { return from_cs != -1 && to_cs != -1; }
    void connect(int from, int to) { from_cs = from; to_cs = to; }

    bool operator==(const Pipe& other) const {
        return id == other.id;
    }
    friend ostream& operator<<(ostream& os, const Pipe& p);

    void print() const;
    void save(ofstream& out) const;
    void load(ifstream& in);
};