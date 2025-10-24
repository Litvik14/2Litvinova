#include "Pipe.h"
#include <iostream>
#include <limits>

using namespace std;

Pipe::Pipe(int id) : id(id) {}

void Pipe::input() {
    cout << "Pipe name: ";
    getline(cin, name);

    cout << "Length: ";
    while (!(cin >> length) || length <= 0) {
        cout << "Error! Enter positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Diametr: ";
    while (!(cin >> diametr) || diametr <= 0) {
        cout << "Error! Enter positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Status (0 - ok, 1 - repair): ";
    while (!(cin >> status) || (status != 0 && status != 1)) {
        cout << "Error! Enter 0 or 1: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();
}

int Pipe::getId() const { return id; }
const string& Pipe::getName() const { return name; }
float Pipe::getLength() const { return length; }
int Pipe::getDiametr() const { return diametr; }
int Pipe::getStatus() const { return status; }

void Pipe::setStatus(int s) {
    if (s == 0 || s == 1) status = s;
}

void Pipe::print() const {
    cout << "ID: " << id
        << " | Name: " << name
        << " | Length: " << length
        << " | Diametr: " << diametr
        << " | Status: " << status << "\n";
}

void Pipe::save(ofstream& out) const {
    out << id << "\n"
        << name << "\n"
        << length << "\n"
        << diametr << "\n"
        << status << "\n";
}

void Pipe::load(ifstream& in) {
    getline(in, name);
    in >> length >> diametr >> status;
    in.ignore();
}