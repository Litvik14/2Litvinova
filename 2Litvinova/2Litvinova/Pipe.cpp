#include "Pipe.h"
#include <iostream>
#include <limits>

Pipe::Pipe(int id) : id(id) {}

void Pipe::input() {
    std::cout << "Pipe name: ";
    std::getline(std::cin, name);

    std::cout << "Length: ";
    while (!(std::cin >> length) || length <= 0) {
        std::cout << "Error! Enter positive number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Diametr: ";
    while (!(std::cin >> diametr) || diametr <= 0) {
        std::cout << "Error! Enter positive integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Status (0 - ok, 1 - repair): ";
    while (!(std::cin >> status) || (status != 0 && status != 1)) {
        std::cout << "Error! Enter 0 or 1: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(); // consume newline
}

int Pipe::getId() const { return id; }
const std::string& Pipe::getName() const { return name; }
float Pipe::getLength() const { return length; }
int Pipe::getDiametr() const { return diametr; }
int Pipe::getStatus() const { return status; }

void Pipe::setStatus(int s) {
    if (s == 0 || s == 1) status = s;
}

void Pipe::print() const {
    std::cout << "ID: " << id
        << " | Name: " << name
        << " | Length: " << length
        << " | Diametr: " << diametr
        << " | Status: " << status << "\n";
}

void Pipe::save(std::ofstream& out) const {
    out << id << "\n"
        << name << "\n"
        << length << "\n"
        << diametr << "\n"
        << status << "\n";
}

void Pipe::load(std::ifstream& in) {
    std::getline(in, name);
    in >> length >> diametr >> status;
    in.ignore();
}