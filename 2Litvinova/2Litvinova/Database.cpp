#include "Database.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

using namespace std;

int Database::addPipe() {
    Pipe p(nextPipeId++);
    p.input();
    pipes[p.getId()] = p;
    Logger::log("Added Pipe ID=" + to_string(p.getId()));
    return p.getId();
}

void Database::editPipe(int id) {
    if (pipes.count(id)) {
        pipes[id].input();
        Logger::log("Edited Pipe ID=" + to_string(id));
    }
    else {
        Logger::log("Edit failed: Pipe ID=" + to_string(id) + " not found");
    }
}

void Database::deletePipe(int id) {
    if (pipes.erase(id)) {
        Logger::log("Deleted Pipe ID=" + to_string(id));
    }
    else {
        Logger::log("Delete failed: Pipe ID=" + to_string(id) + " not found");
    }
}

vector<int> Database::findPipesByName(const string& name) {
    vector<int> result;
    if (name.empty()) return result;
    for (const auto& [id, pipe] : pipes) {
        if (pipe.getName().find(name) != string::npos) {
            result.push_back(id);
        }
    }
    Logger::log("Searched pipes by name '" + name + "', found: " + to_string(result.size()));
    return result;
}

vector<int> Database::findPipesByStatus(int status) {
    vector<int> result;
    for (const auto& [id, pipe] : pipes) {
        if (pipe.getStatus() == status) {
            result.push_back(id);
        }
    }
    Logger::log("Searched pipes by status=" + to_string(status) + ", found: " + to_string(result.size()));
    return result;
}

void Database::printAllPipes() const {
    if (pipes.empty()) {
        cout << "No pipes.\n";
        return;
    }
    for (const auto& [id, pipe] : pipes) {
        pipe.print();
    }
}

void Database::savePipes(const string& filename) const {
    ofstream out(filename);
    if (!out) {
        Logger::log("ERROR: Cannot open pipe file " + filename);
        return;
    }
    out << pipes.size() << "\n";
    for (const auto& [id, pipe] : pipes) {
        pipe.save(out);
    }
    Logger::log("Saved " + to_string(pipes.size()) + " pipes to " + filename);
}

void Database::loadPipes(const string& filename) {
    ifstream in(filename);
    if (!in) {
        Logger::log("Pipe file " + filename + " not found, skipping load.");
        return;
    }
    size_t count;
    in >> count;
    in.ignore();
    pipes.clear();
    for (size_t i = 0; i < count; ++i) {
        int id;
        in >> id;
        in.ignore();
        Pipe p(id);
        p.load(in);
        pipes[id] = p;
        if (id >= nextPipeId) nextPipeId = id + 1;
    }
    Logger::log("Loaded " + to_string(pipes.size()) + " pipes from " + filename);
}

int Database::addStation() {
    CompressorStation cs(nextStationId++);
    cs.input();
    stations[cs.getId()] = cs;
    Logger::log("Added Station ID=" + to_string(cs.getId()));
    return cs.getId();
}

void Database::editStation(int id) {
    if (stations.count(id)) {
        stations[id].input();
        Logger::log("Edited Station ID=" + to_string(id));
    }
    else {
        Logger::log("Edit failed: Station ID=" + to_string(id) + " not found");
    }
}

void Database::deleteStation(int id) {
    if (stations.erase(id)) {
        Logger::log("Deleted Station ID=" + to_string(id));
    }
    else {
        Logger::log("Delete failed: Station ID=" + to_string(id) + " not found");
    }
}

vector<int> Database::findStationsByName(const string& name) {
    vector<int> result;
    if (name.empty()) return result;
    for (const auto& [id, cs] : stations) {
        if (cs.getName().find(name) != string::npos) {
            result.push_back(id);
        }
    }
    Logger::log("Searched stations by name '" + name + "', found: " + to_string(result.size()));
    return result;
}

vector<int> Database::findStationsByIdlePercent(float minIdle, float maxIdle) {
    vector<int> result;
    for (const auto& [id, cs] : stations) {
        float idle = cs.getIdlePercent();
        if (idle >= minIdle && idle <= maxIdle) {
            result.push_back(id);
        }
    }
    Logger::log("Searched stations by idle% [" + to_string(minIdle) + ", " + to_string(maxIdle) + "], found: " + to_string(result.size()));
    return result;
}

void Database::printAllStations() const {
    if (stations.empty()) {
        cout << "No stations.\n";
        return;
    }
    for (const auto& [id, cs] : stations) {
        cs.print();
    }
}

void Database::saveStations(const string& filename) const {
    ofstream out(filename);
    if (!out) {
        Logger::log("ERROR: Cannot open station file " + filename);
        return;
    }
    out << stations.size() << "\n";
    for (const auto& [id, cs] : stations) {
        cs.save(out);
    }
    Logger::log("Saved " + to_string(stations.size()) + " stations to " + filename);
}

void Database::loadStations(const string& filename) {
    ifstream in(filename);
    if (!in) {
        Logger::log("Station file " + filename + " not found, skipping load.");
        return;
    }
    size_t count;
    in >> count;
    in.ignore();
    stations.clear();
    for (size_t i = 0; i < count; ++i) {
        int id;
        in >> id;
        in.ignore();
        CompressorStation cs(id);
        cs.load(in);
        stations[id] = cs;
        if (id >= nextStationId) nextStationId = id + 1;
    }
    Logger::log("Loaded " + to_string(stations.size()) + " stations from " + filename);
}

void Database::batchEditPipes(const vector<int>& ids) {
    if (ids.empty()) return;
    cout << "Set new status (0=ok, 1=repair) for selected pipes: ";
    int newStatus;
    while (!(cin >> newStatus) || (newStatus != 0 && newStatus != 1)) {
        cout << "Enter 0 or 1: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    for (int id : ids) {
        if (pipes.count(id)) {
            pipes[id].setStatus(newStatus);
            Logger::log("Batch edited Pipe ID=" + to_string(id) + " status=" + to_string(newStatus));
        }
    }
    cin.ignore();
}

void Database::batchDeletePipes(const vector<int>& ids) {
    for (int id : ids) {
        deletePipe(id);
    }
}