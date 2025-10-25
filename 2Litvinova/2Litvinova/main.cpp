#include "Pipe.h"
#include "CompressorStation.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <sstream>
#include <limits>
#include <unordered_map>

using namespace std;

unordered_map<int, Pipe> pipes;
unordered_map<int, CompressorStation> stations;
int nextPipeId = 1;
int nextStationId = 1;

int addPipe() {
    Pipe p(nextPipeId++);
    p.input();
    pipes[p.getId()] = p;
    logAction("Added Pipe ID=" + to_string(p.getId()));
    return p.getId();
}

void deletePipe(int id) {
    if (pipes.erase(id)) {
        logAction("Deleted Pipe ID=" + to_string(id));
    }
    else {
        logAction("Delete failed: Pipe ID=" + to_string(id) + " not found");
    }
}

vector<int> findPipesByName(const string& name) {
    vector<int> result;
    if (name.empty()) return result;
    for (const auto& p : pipes) {
        if (p.second.getName().find(name) != string::npos) {
            result.push_back(p.first);
        }
    }
    logAction("Searched pipes by name '" + name + "', found: " + to_string(result.size()));
    return result;
}

vector<int> findPipesByStatus(int status) {
    vector<int> result;
    for (const auto& p : pipes) {
        if (p.second.getStatus() == status) {
            result.push_back(p.first);
        }
    }
    logAction("Searched pipes by status=" + to_string(status) + ", found: " + to_string(result.size()));
    return result;
}

void batchEditPipes(const vector<int>& ids) {
    if (ids.empty()) return;
    int newStatus = inputInt("Set new status (0=ok, 1=repair): ", 0, 1);
    for (int id : ids) {
        if (pipes.count(id)) {
            pipes[id].setStatus(newStatus);
            logAction("Batch edited Pipe ID=" + to_string(id) + " status=" + to_string(newStatus));
        }
    }
}

void printAllPipes() {
    if (pipes.empty()) { cout << "No pipes.\n"; return; }
    vector<int> ids;
    for (const auto& p : pipes) ids.push_back(p.first);
    sort(ids.begin(), ids.end());
    for (int id : ids) pipes[id].print();
}

void savePipes(const string& filename) {
    ofstream out(filename);
    if (!out) { logAction("ERROR: Cannot open pipe file " + filename); return; }
    out << pipes.size() << "\n";
    for (const auto& p : pipes) p.second.save(out);
    logAction("Saved " + to_string(pipes.size()) + " pipes to " + filename);
}

void loadPipes(const string& filename) {
    ifstream in(filename);
    if (!in) { logAction("Pipe file " + filename + " not found, skipping load."); return; }
    size_t count; in >> count; in.ignore();
    pipes.clear();
    for (size_t i = 0; i < count; ++i) {
        int id; in >> id; in.ignore();
        Pipe p(id); p.load(in);
        pipes[id] = p;
        if (id >= nextPipeId) nextPipeId = id + 1;
    }
    logAction("Loaded " + to_string(pipes.size()) + " pipes from " + filename);
}

// === Работа с КС ===

int addStation() {
    CompressorStation cs(nextStationId++);
    cs.input();
    stations[cs.getId()] = cs;
    logAction("Added Station ID=" + to_string(cs.getId()));
    return cs.getId();
}

vector<int> findStationsByName(const string& name) {
    vector<int> result;
    if (name.empty()) return result;
    for (const auto& cs : stations) {
        if (cs.second.getName().find(name) != string::npos) {
            result.push_back(cs.first);
        }
    }
    logAction("Searched stations by name '" + name + "', found: " + to_string(result.size()));
    return result;
}

vector<int> findStationsByIdlePercent(float minIdle, float maxIdle) {
    vector<int> result;
    for (const auto& cs : stations) {
        float idle = cs.second.getIdlePercent();
        if (idle >= minIdle && idle <= maxIdle) {
            result.push_back(cs.first);
        }
    }
    logAction("Searched stations by idle% [" + to_string(minIdle) + ", " + to_string(maxIdle) + "], found: " + to_string(result.size()));
    return result;
}

void printAllStations() {
    if (stations.empty()) { cout << "No stations.\n"; return; }
    vector<int> ids;
    for (const auto& cs : stations) ids.push_back(cs.first);
    sort(ids.begin(), ids.end());
    for (int id : ids) stations[id].print();
}

void saveStations(const string& filename) {
    ofstream out(filename);
    if (!out) { logAction("ERROR: Cannot open station file " + filename); return; }
    out << stations.size() << "\n";
    for (const auto& cs : stations) cs.second.save(out);
    logAction("Saved " + to_string(stations.size()) + " stations to " + filename);
}

void loadStations(const string& filename) {
    ifstream in(filename);
    if (!in) { logAction("Station file " + filename + " not found, skipping load."); return; }
    size_t count; in >> count; in.ignore();
    stations.clear();
    for (size_t i = 0; i < count; ++i) {
        int id; in >> id; in.ignore();
        CompressorStation cs(id); cs.load(in);
        stations[id] = cs;
        if (id >= nextStationId) nextStationId = id + 1;
    }
    logAction("Loaded " + to_string(stations.size()) + " stations from " + filename);
}

// === Логирование (упрощённое) ===

string logFilename = "pipeline.log";

void setLogFile(const string& filename) {
    logFilename = filename;
}

void logAction(const string& message) {
    time_t now = time(0);
    char dt[26];
    ctime_s(dt, sizeof(dt), &now); // безопасная версия
    dt[strlen(dt) - 1] = '\0'; // убрать \n

    ofstream logFile(logFilename, ios::app);
    logFile << "[" << dt << "] " << message << "\n";
    logFile.close();

    cout << "[LOG] " << message << "\n";
}

// === Реализация utils функций (в main.cpp, чтобы не было отдельного utils.cpp) ===

string inputString(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

int inputInt(const string& prompt, int min, int max) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            if ((min == -1 || val >= min) && (max == -1 || val <= max)) {
                cin.ignore();
                return val;
            }
        }
        cout << "Invalid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

float inputFloat(const string& prompt, float min) {
    float val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= min) {
            cin.ignore();
            return val;
        }
        cout << "Invalid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

vector<int> selectIdsFromList(const vector<int>& candidates) {
    if (candidates.empty()) return {};
    cout << "Found IDs: ";
    for (size_t i = 0; i < candidates.size(); ++i) {
        cout << candidates[i];
        if (i < candidates.size() - 1) cout << ", ";
    }
    cout << "\nSelect IDs to edit (space-separated), or type 'all': ";
    string line;
    getline(cin, line);
    if (line == "all") return candidates;

    vector<int> selected;
    istringstream iss(line);
    int id;
    while (iss >> id) {
        if (find(candidates.begin(), candidates.end(), id) != candidates.end()) {
            selected.push_back(id);
        }
    }
    return selected;
}

// === Главная функция ===

int main() {
    string pipeFile = inputString("Enter pipe data filename: ");
    string stationFile = inputString("Enter station data filename: ");
    setLogFile("pipeline.log");

    loadPipes(pipeFile);
    loadStations(stationFile);

    while (true) {
        cout << "\n=== MENU ===\n"
            << "1. Add pipe\n"
            << "2. Add CS\n"
            << "3. View all\n"
            << "4. Search pipes\n"
            << "5. Search CS\n"
            << "6. Save & Exit\n";
        int choice = inputInt("Choice: ", 1, 6);

        switch (choice) {
        case 1: addPipe(); break;
        case 2: addStation(); break;
        case 3:
            cout << "\n--- PIPES ---\n"; printAllPipes();
            cout << "\n--- STATIONS ---\n"; printAllStations();
            break;
        case 4: {
            string name = inputString("Search pipe by name (empty to skip): ");
            vector<int> candidates = findPipesByName(name);
            if (inputString("Filter by status? (y/n): ") == "y") {
                int status = inputInt("Status (0=ok,1=repair): ", 0, 1);
                vector<int> byStatus = findPipesByStatus(status);
                sort(candidates.begin(), candidates.end());
                sort(byStatus.begin(), byStatus.end());
                vector<int> intersect;
                set_intersection(candidates.begin(), candidates.end(),
                    byStatus.begin(), byStatus.end(),
                    back_inserter(intersect));
                candidates = intersect;
            }
            if (candidates.empty()) {
                cout << "No pipes found.\n";
            }
            else {
                cout << "Found " << candidates.size() << " pipe(s):\n";
                for (int id : candidates) pipes[id].print();
                string act = inputString("Action: (e)dit, (d)elete, (c)ancel: ");
                if (act == "e") {
                    vector<int> sel = selectIdsFromList(candidates);
                    batchEditPipes(sel);
                }
                else if (act == "d") {
                    vector<int> sel = selectIdsFromList(candidates);
                    for (int id : sel) deletePipe(id);
                }
            }
            break;
        }
        case 5: {
            string name = inputString("Search CS by name (empty to skip): ");
            vector<int> candidates = findStationsByName(name);
            if (inputString("Filter by idle %? (y/n): ") == "y") {
                float minI = inputFloat("Min idle %: ", 0.0f);
                float maxI = inputFloat("Max idle %: ", minI);
                vector<int> byIdle = findStationsByIdlePercent(minI, maxI);
                sort(candidates.begin(), candidates.end());
                sort(byIdle.begin(), byIdle.end());
                vector<int> intersect;
                set_intersection(candidates.begin(), candidates.end(),
                    byIdle.begin(), byIdle.end(),
                    back_inserter(intersect));
                candidates = intersect;
            }
            if (candidates.empty()) {
                cout << "No stations found.\n";
            }
            else {
                cout << "Found " << candidates.size() << " station(s):\n";
                for (int id : candidates) stations[id].print();
            }
            break;
        }
        case 6:
            savePipes(pipeFile);
            saveStations(stationFile);
            logAction("Saved data and exited.");
            cout << "Goodbye!\n";
            return 0;
        }
    }
}