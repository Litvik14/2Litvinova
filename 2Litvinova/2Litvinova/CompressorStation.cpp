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

    cout << "Number of workshops: ";
    while (!(cin >> number_work) || number_work <= 0) {
        cout << "Error! Enter positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Workshops in operation: ";
    while (!(cin >> number_work_online) || number_work_online < 0 || number_work_online > number_work) {
        cout << "Error! Enter number between 0 and " << number_work << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "CS class: ";
    cin.ignore();
    getline(cin, class_cs);
}

void CompressorStation::print() const {
    cout << "ID: " << id
        << " | Name: " << name
        << " | Workshops: " << number_work
        << " | Online: " << number_work_online
        << " | Class: " << class_cs
        << " | Idle: " << getIdlePercent() << "%\n";
}

void CompressorStation::save(ofstream& out) const {
    out << id << "\n" << name << "\n" << number_work << "\n" << number_work_online << "\n" << class_cs << "\n";
}

void CompressorStation::load(ifstream& in) {
    getline(in, name);
    in >> number_work >> number_work_online;
    in.ignore();
    getline(in, class_cs);
}