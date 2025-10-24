#pragma once
#include <string>
#include <fstream>

class Pipe {
private:
    int id;
    std::string name;
    float length;
    int diametr;
    int status;

public:
    Pipe(int id = -1);
    void input();
    int getId() const;
    const std::string& getName() const;
    float getLength() const;
    int getDiametr() const;
    int getStatus() const;
    void setStatus(int s);
    void print() const;
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
};