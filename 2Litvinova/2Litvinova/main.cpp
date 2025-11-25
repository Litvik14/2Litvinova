#include "Pipe.h"
#include "CompressorStation.h"
#include "utils.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <set>
#include <map>
using namespace std;

unordered_map<int, vector<int>> network_graph;
unordered_map<int, int> vertex_in_degree;
set<int> all_stations;

void addConnectionToGraph(int from, int to) {
    network_graph[from].push_back(to);
    vertex_in_degree[to]++;
    all_stations.insert(from);
    all_stations.insert(to);
    if (vertex_in_degree.find(from) == vertex_in_degree.end()) {
        vertex_in_degree[from] = 0;
    }
}

void updateFreePipes(const unordered_map<int, Pipe>& pipes, map<int, vector<int>>& freeMap) {
    freeMap.clear();
    set<int> allowed = { 500, 700, 1000, 1400 };
    for (const auto& item : pipes) {
        const Pipe& p = item.second;
        if (!p.isConnected() && allowed.find(p.getDiametr()) != allowed.end()) {
            freeMap[p.getDiametr()].push_back(p.getId());
        }
    }
}

void connectStations(
    unordered_map<int, Pipe>& pipes,
    unordered_map<int, CompressorStation>& stations,
    map<int, vector<int>>& free_pipes_by_diametr,
    int from_cs_id, int to_cs_id, int diametr) {

    if (!containsId(stations, from_cs_id) || !containsId(stations, to_cs_id)) {
        cout << "Error: One of the stations not found!\n";
        return;
    }

    if (free_pipes_by_diametr[diametr].empty()) {
        static int nid = 1;
        Pipe p(nid++);
        string name = "Pipe_" + to_string(from_cs_id) + "_" + to_string(to_cs_id);
        p.set(name, 100.0f, diametr, 0, from_cs_id, to_cs_id);
        pipes[p.getId()] = p;
        stations.at(from_cs_id).addOutputPipe(p.getId());
        stations.at(to_cs_id).addInputPipe(p.getId());
        cout << "Created new pipe ID=" << p.getId() << "\n";
    }
    else {
        int pipe_id = free_pipes_by_diametr[diametr].back();
        free_pipes_by_diametr[diametr].pop_back();
        pipes.at(pipe_id).connect(from_cs_id, to_cs_id);
        stations.at(from_cs_id).addOutputPipe(pipe_id);
        stations.at(to_cs_id).addInputPipe(pipe_id);
        cout << "Connected using existing pipe ID=" << pipe_id << "\n";
    }

    addConnectionToGraph(from_cs_id, to_cs_id);
    updateFreePipes(pipes, free_pipes_by_diametr);
}

vector<int> topologicalSort() {
    unordered_map<int, int> in_deg = vertex_in_degree;
    queue<int> q;
    vector<int> result;

    for (int id : all_stations) {
        if (in_deg[id] == 0) {
            q.push(id);
        }
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        result.push_back(u);
        if (network_graph.find(u) != network_graph.end()) {
            for (int v : network_graph.at(u)) {
                in_deg[v]--;
                if (in_deg[v] == 0) {
                    q.push(v);
                }
            }
        }
    }

    return result;
}

vector<int> findPipes(const unordered_map<int, Pipe>& pipes, const string& namePart = "", int status = -1) {
    vector<int> ids;
    for (const auto& item : pipes) {
        const Pipe& p = item.second;
        bool nameMatch = namePart.empty() || (p.getName().find(namePart) != string::npos);
        bool statusMatch = (status == -1) || (p.getStatus() == status);
        if (nameMatch && statusMatch) {
            ids.push_back(p.getId());
        }
    }
    return ids;
}

vector<int> findStations(const unordered_map<int, CompressorStation>& stations, const string& namePart = "", float minIdle = -1.0f) {
    vector<int> ids;
    for (const auto& item : stations) {
        const CompressorStation& s = item.second;
        bool nameMatch = namePart.empty() || (s.getName().find(namePart) != string::npos);
        bool idleMatch = (minIdle < 0) || (s.getIdlePercent() >= minIdle);
        if (nameMatch && idleMatch) {
            ids.push_back(s.getId());
        }
    }
    return ids;
}

void batchEditPipes(unordered_map<int, Pipe>& pipes, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Edit all found pipes?")) {
        for (int id : ids) {
            cout << "\nEditing pipe ID=" << id << "\n";
            pipes.at(id).input();
        }
    }
    else {
        string line = inputString("Enter IDs to edit (space-separated): ");
        vector<int> sel = parseIds(line);
        for (int id : sel) {
            if (pipes.count(id)) {
                pipes.at(id).input();
            }
        }
    }
}

void batchDeletePipes(unordered_map<int, Pipe>& pipes, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Delete all found pipes?")) {
        for (int id : ids) {
            pipes.erase(id);
        }
        cout << "Deleted " << ids.size() << " pipe(s).\n";
    }
    else {
        string line = inputString("Enter IDs to delete (space-separated): ");
        vector<int> sel = parseIds(line);
        int cnt = 0;
        for (int id : sel) {
            if (pipes.erase(id)) cnt++;
        }
        cout << "Deleted " << cnt << " pipe(s).\n";
    }
}

void batchEditStations(unordered_map<int, CompressorStation>& stations, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Edit all found stations?")) {
        for (int id : ids) {
            cout << "\nEditing station ID=" << id << "\n";
            stations.at(id).input();
        }
    }
    else {
        string line = inputString("Enter IDs to edit (space-separated): ");
        vector<int> sel = parseIds(line);
        for (int id : sel) {
            if (stations.count(id)) {
                stations.at(id).input();
            }
        }
    }
}

void batchDeleteStations(unordered_map<int, CompressorStation>& stations, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Delete all found stations?")) {
        for (int id : ids) {
            stations.erase(id);
        }
        cout << "Deleted " << ids.size() << " station(s).\n";
    }
    else {
        string line = inputString("Enter IDs to delete (space-separated): ");
        vector<int> sel = parseIds(line);
        int cnt = 0;
        for (int id : sel) {
            if (stations.erase(id)) cnt++;
        }
        cout << "Deleted " << cnt << " station(s).\n";
    }
}

void printAll(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CompressorStation>& stations) {
    if (pipes.empty() && stations.empty()) {
        cout << "No data.\n";
        return;
    }
    if (!pipes.empty()) {
        cout << "\n--- PIPES ---\n";
        printAllObjects(pipes);
    }
    if (!stations.empty()) {
        cout << "\n--- COMPRESSOR STATIONS ---\n";
        printAllObjects(stations);
    }
}

void saveAll(const unordered_map<int, Pipe>& pipes, const unordered_map<int, CompressorStation>& stations) {
    string fname = inputString("Enter filename to save: ");
    if (fname.empty()) fname = "data.txt";
    ofstream out(fname);
    if (!out) {
        cout << "Save failed.\n";
        return;
    }
    out << pipes.size() << "\n";
    for (const auto& item : pipes) item.second.save(out);
    out << stations.size() << "\n";
    for (const auto& item : stations) item.second.save(out);
    cout << "Saved to '" << fname << "'\n";
}

void loadAll(unordered_map<int, Pipe>& pipes, unordered_map<int, CompressorStation>& stations) {
    string fname = inputString("Enter filename to load: ");
    if (fname.empty()) fname = "data.txt";
    ifstream in(fname);
    if (!in) {
        cout << "File not found.\n";
        return;
    }
    size_t n;
    pipes.clear(); stations.clear();
    network_graph.clear();
    vertex_in_degree.clear();
    all_stations.clear();

    in >> n; in.ignore();
    for (size_t i = 0; i < n; ++i) {
        Pipe p(0); p.load(in); pipes[p.getId()] = p;
        if (p.isConnected()) {
            addConnectionToGraph(p.getFromCS(), p.getToCS());
        }
    }
    in >> n; in.ignore();
    for (size_t i = 0; i < n; ++i) {
        CompressorStation s(0); s.load(in); stations[s.getId()] = s;
    }
    cout << "Loaded from '" << fname << "'\n";
}

int main() {
    unordered_map<int, Pipe> pipes;
    unordered_map<int, CompressorStation> stations;
    map<int, vector<int>> free_pipes_by_diametr;

    while (true) {
        cout << "\n--- MAIN MENU ---\n"
            << "1. Add pipe\n"
            << "2. Add compressor station\n"
            << "3. Show all\n"
            << "4. Search pipes\n"
            << "5. Search stations\n"
            << "6. Save\n"
            << "7. Load\n"
            << "8. Connect stations\n"
            << "9. Topological sort\n"
            << "10. Exit\n"
            << "Choice: ";

        int choice = inputInt("");
        vector<int> foundIds;

        switch (choice) {
        case 1: {
            static int nid = 1;
            Pipe p(nid++);
            p.input();
            pipes[p.getId()] = p;
            cout << "Pipe added (ID=" << p.getId() << ")\n";
            updateFreePipes(pipes, free_pipes_by_diametr);
            break;
        }
        case 2: {
            static int nid = 1;
            CompressorStation s(nid++);
            s.input();
            stations[s.getId()] = s;
            cout << "Station added (ID=" << s.getId() << ")\n";
            break;
        }
        case 3:
            printAll(pipes, stations);
            break;

        case 4: {
            string name = inputString("Pipe name (empty to skip): ");
            string statusStr = inputString("Status (0=ok, 1=repair, empty=all): ");
            int status = -1;
            if (!statusStr.empty()) status = stoi(statusStr);
            foundIds = findPipes(pipes, name, status);
            if (foundIds.empty()) {
                cout << "No pipes found.\n";
            }
            else {
                cout << "\nFound " << foundIds.size() << " pipe(s):\n";
                for (int id : foundIds) {
                    cout << pipes.at(id) << "\n";
                }
                int op = inputInt("Actions: 1=Edit, 2=Delete, 0=Back: ");
                if (op == 1) batchEditPipes(pipes, foundIds);
                else if (op == 2) batchDeletePipes(pipes, foundIds);
            }
            break;
        }

        case 5: {
            string name = inputString("Station name (empty to skip): ");
            string idleStr = inputString("Min idle % (empty to skip): ");
            float minIdle = -1.0f;
            if (!idleStr.empty()) minIdle = stof(idleStr);
            foundIds = findStations(stations, name, minIdle);
            if (foundIds.empty()) {
                cout << "No stations found.\n";
            }
            else {
                cout << "\nFound " << foundIds.size() << " station(s):\n";
                for (int id : foundIds) {
                    cout << stations.at(id) << "\n";
                }
                int op = inputInt("Actions: 1=Edit, 2=Delete, 0=Back: ");
                if (op == 1) batchEditStations(stations, foundIds);
                else if (op == 2) batchDeleteStations(stations, foundIds);
            }
            break;
        }

        case 6: saveAll(pipes, stations); break;
        case 7: loadAll(pipes, stations); updateFreePipes(pipes, free_pipes_by_diametr); break;

        case 8: {
            int from = inputInt("From CS ID: ");
            int to = inputInt("To CS ID: ");
            int diam = inputInt("Diameter (500/700/1000/1400): ");
            set<int> allowed = { 500, 700, 1000, 1400 };
            if (allowed.find(diam) == allowed.end()) {
                cout << "Invalid diameter! Use 500, 700, 1000 or 1400.\n";
                break;
            }
            connectStations(pipes, stations, free_pipes_by_diametr, from, to, diam);
            break;
        }

        case 9: {
            vector<int> topo = topologicalSort();
            if (topo.size() != all_stations.size()) {
                cout << "Network has cycles! Topological sort not possible.\n";
            }
            else {
                cout << "Topological order: ";
                for (int id : topo) cout << id << " ";
                cout << "\n";
            }
            break;
        }

        case 10:
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid option.\n";
        }
    }
}