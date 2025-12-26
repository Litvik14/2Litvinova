#include "CompressorStation.h"
#include <iostream>
#include <limits>
using namespace std;

CompressorStation::CompressorStation(int id)
    : id(id), name(""), number_work(0), number_work_online(0), class_cs("") {
}

void CompressorStation::input() {
    cout << "CS name: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Error! Name cannot be empty. Please enter a name: ";
        getline(cin, name);
    }
    cerr << name << endl;

    cout << "Number of workshops: ";
    cin >> number_work;
    while (number_work <= 0 || cin.fail() || (cin.peek() != '\n')) {
        cout << "Error! Please, enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number_work;
    }
    cin.ignore();
    cerr << number_work << endl;

    cout << "Workshops in operation: ";
    cin >> number_work_online;
    while (number_work_online < 0 || cin.fail() || number_work_online > number_work || (cin.peek() != '\n')) {
        cout << "Error! Please, enter a positive number no more than the number of CG workshops: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> number_work_online;
    }
    cin.ignore();
    cerr << number_work_online << endl;

    cout << "CS class: ";
    getline(cin, class_cs);
    while (class_cs.empty()) {
        cout << "Error! Class cannot be empty. Please enter a class: ";
        getline(cin, class_cs);
    }
    cerr << class_cs << endl;
}

ostream& operator<<(ostream& os, const CompressorStation& cs) {
    os << "ID: " << cs.id
        << " | Name: " << cs.name
        << " | Workshops: " << cs.number_work
        << " | Online: " << cs.number_work_online
        << " | Class: " << cs.class_cs
        << " | Idle: " << cs.getIdlePercent() << "%";
    return os;
}

void CompressorStation::print() const {
    cout << *this << "\n";
}

void CompressorStation::save(ofstream& out) const {
    out << id << "\n" << name << "\n" << number_work << "\n" << number_work_online << "\n" << class_cs << "\n";
}

void CompressorStation::load(ifstream& in) {
    in >> id;
    in.ignore();
    getline(in, name);
    in >> number_work >> number_work_online;
    in.ignore();
    getline(in, class_cs);
}