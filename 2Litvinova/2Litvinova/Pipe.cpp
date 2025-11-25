#include "Pipe.h"
#include <iostream>
#include <limits>
using namespace std;

Pipe::Pipe(int id) : id(id), name(""), length(0.0f), diametr(0), status(0), from_cs(-1), to_cs(-1) {}

void Pipe::input() {
    cout << "Pipe name: ";
    getline(cin, name);
    cerr << name << endl;

    cout << "Length: ";
    cin >> length;
    while (cin.fail() || length <= 0 || cin.peek() != '\n') {
        cout << "Error! Please, enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> length;
    }
    cin.ignore();
    cerr << length << endl;

    cout << "Diametr: ";
    cin >> diametr;
    while (cin.fail() || diametr <= 0 || cin.peek() != '\n') {
        cout << "Error! Please, enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> diametr;
    }
    cin.ignore();
    cerr << diametr << endl;

    cout << "Status (0 - ok, 1 - repair): ";
    cin >> status;
    while (cin.fail() || (status != 0 && status != 1) || cin.peek() != '\n') {
        cout << "Error! Please, enter 0 or 1: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> status;
    }
    cin.ignore();
    cerr << status << endl;
}

ostream& operator<<(ostream& os, const Pipe& p) {
    os << "ID: " << p.id
        << " | Name: " << p.name
        << " | Length: " << p.length
        << " | Diametr: " << p.diametr
        << " | Status: " << p.status;
    if (p.isConnected()) {
        os << " | From CS: " << p.from_cs << " | To CS: " << p.to_cs;
    }
    return os;
}

void Pipe::print() const {
    cout << *this << "\n";
}

void Pipe::save(ofstream& out) const {
    out << id << "\n" << name << "\n" << length << "\n" << diametr << "\n" << status << "\n"
        << from_cs << "\n" << to_cs << "\n";
}

void Pipe::load(ifstream& in) {
    in >> id >> length >> diametr >> status >> from_cs >> to_cs;
    in.ignore();
    getline(in, name);
}