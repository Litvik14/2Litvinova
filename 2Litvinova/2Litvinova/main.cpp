#include "GasNetwork.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

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

void batchEditPipes(GasNetwork& net, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Edit all found pipes?")) {
        for (int id : ids) {
            cout << "\nEditing pipe ID=" << id << "\n";
            Pipe p = net.getPipes().at(id);
            p.input();
            net.addPipe(p);
        }
    }
    else {
        string line = inputString("Enter IDs to edit (space-separated): ");
        vector<int> sel = parseIds(line);
        for (int id : sel) {
            if (net.getPipes().count(id)) {
                Pipe p = net.getPipes().at(id);
                p.input();
                net.addPipe(p);
            }
        }
    }
}

void batchEditStations(GasNetwork& net, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Edit all found stations?")) {
        for (int id : ids) {
            cout << "\nEditing station ID=" << id << "\n";
            CompressorStation s = net.getStations().at(id);
            s.input();
            net.addStation(s);
        }
    }
    else {
        string line = inputString("Enter IDs to edit (space-separated): ");
        vector<int> sel = parseIds(line);
        for (int id : sel) {
            if (net.getStations().count(id)) {
                CompressorStation s = net.getStations().at(id);
                s.input();
                net.addStation(s);
            }
        }
    }
}

void batchDeletePipes(GasNetwork& net, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Delete all found pipes?")) {
        for (int id : ids) net.deletePipe(id);
        cout << "Deleted " << ids.size() << " pipe(s).\n";
    }
    else {
        string line = inputString("Enter IDs to delete (space-separated): ");
        vector<int> sel = parseIds(line);
        int cnt = 0;
        for (int id : sel) {
            if (net.getPipes().count(id)) {
                net.deletePipe(id);
                cnt++;
            }
        }
        cout << "Deleted " << cnt << " pipe(s).\n";
    }
}

void batchDeleteStations(GasNetwork& net, const vector<int>& ids) {
    if (ids.empty()) return;
    if (confirm("Delete all found stations?")) {
        for (int id : ids) net.deleteStation(id);
        cout << "Deleted " << ids.size() << " station(s).\n";
    }
    else {
        string line = inputString("Enter IDs to delete (space-separated): ");
        vector<int> sel = parseIds(line);
        int cnt = 0;
        for (int id : sel) {
            if (net.getStations().count(id)) {
                net.deleteStation(id);
                cnt++;
            }
        }
        cout << "Deleted " << cnt << " station(s).\n";
    }
}

void saveAll(const GasNetwork& network, const string& fname) {
    ofstream out(fname);
    if (!out) {
        cout << "Save failed.\n";
        return;
    }
    const auto& pipes = network.getPipes();
    const auto& stations = network.getStations();
    out << pipes.size() << "\n";
    for (const auto& item : pipes) item.second.save(out);
    out << stations.size() << "\n";
    for (const auto& item : stations) item.second.save(out);
    cout << "Saved to '" << fname << "'\n";
}

void loadAll(GasNetwork& network, const string& fname) {
    ifstream in(fname);
    if (!in) {
        cout << "File not found.\n";
        return;
    }
    size_t n;
    in >> n;
    in.ignore();
    for (size_t i = 0; i < n; ++i) {
        Pipe p(0); p.load(in); network.addPipe(p);
    }
    in >> n;
    in.ignore();
    for (size_t i = 0; i < n; ++i) {
        CompressorStation s(0); s.load(in); network.addStation(s);
    }
    cout << "Loaded from '" << fname << "'\n";
}

int main() {
    GasNetwork network;

    while (true) {
        cout << "\n--- MAIN MENU ---\n"
            << "1. Add pipe\n"
            << "2. Add compressor station\n"
            << "3. Show all\n"
            << "4. Edit pipe\n"
            << "5. Edit station\n"
            << "6. Delete pipe\n"
            << "7. Delete station\n"
            << "8. Search pipes\n"
            << "9. Search stations\n"
            << "10. Connect stations\n"
            << "11. Topological sort\n"
            << "12. Max flow\n"
            << "13. Shortest path\n"
            << "14. Save\n"
            << "15. Load\n"
            << "16. Exit\n"
            << "Choice: ";

        int choice = inputInt("");

        switch (choice) {
        case 1: {
            Pipe p(-1);
            p.input();
            network.addPipe(p);
            break;
        }
        case 2: {
            CompressorStation s(-1);
            s.input();
            network.addStation(s);
            break;
        }
        case 3: {
            const auto& pipes = network.getPipes();
            const auto& stations = network.getStations();

            if (pipes.empty() && stations.empty()) {
                cout << "No data.\n";
            }
            else {
                if (!pipes.empty()) {
                    cout << "\n--- PIPES ---\n";
                    printAllObjects(pipes);
                }
                else {
                    cout << "\nNo pipes.\n";
                }

                if (!stations.empty()) {
                    cout << "\n--- COMPRESSOR STATIONS ---\n";
                    printAllObjects(stations);
                }
                else {
                    cout << "\nNo stations.\n";
                }

                network.printNetwork();
            }
            break;
        }
        case 4: {
            int id = inputInt("Pipe ID to edit: ");
            if (network.getPipes().count(id)) {
                Pipe p = network.getPipes().at(id);
                p.input();
                network.addPipe(p);
            }
            else {
                cout << "Pipe not found.\n";
            }
            break;
        }
        case 5: {
            int id = inputInt("Station ID to edit: ");
            if (network.getStations().count(id)) {
                CompressorStation s = network.getStations().at(id);
                s.input();
                network.addStation(s);
            }
            else {
                cout << "Station not found.\n";
            }
            break;
        }
        case 6: {
            int id = inputInt("Pipe ID to delete: ");
            network.deletePipe(id);
            cout << "Pipe deleted.\n";
            break;
        }
        case 7: {
            int id = inputInt("Station ID to delete: ");
            network.deleteStation(id);
            cout << "Station deleted.\n";
            break;
        }
        case 8: {
            string name = inputString("Pipe name (empty to skip): ");
            string statusStr = inputString("Status (0=ok, 1=repair, empty=all): ");
            int status = -1;
            if (!statusStr.empty()) status = stoi(statusStr);
            vector<int> found = findPipes(network.getPipes(), name, status);
            if (found.empty()) {
                cout << "No pipes found.\n";
            }
            else {
                cout << "\nFound " << found.size() << " pipe(s):\n";
                for (int id : found) cout << network.getPipes().at(id) << "\n";
                int op = inputInt("Actions: 1=Edit, 2=Delete, 0=Back: ");
                if (op == 1) batchEditPipes(network, found);
                else if (op == 2) batchDeletePipes(network, found);
            }
            break;
        }
        case 9: {
            string name = inputString("Station name (empty to skip): ");
            string idleStr = inputString("Min idle % (empty to skip): ");
            float minIdle = -1.0f;
            if (!idleStr.empty()) minIdle = stof(idleStr);
            vector<int> found = findStations(network.getStations(), name, minIdle);
            if (found.empty()) {
                cout << "No stations found.\n";
            }
            else {
                cout << "\nFound " << found.size() << " station(s):\n";
                for (int id : found) cout << network.getStations().at(id) << "\n";
                int op = inputInt("Actions: 1=Edit, 2=Delete, 0=Back: ");
                if (op == 1) batchEditStations(network, found);
                else if (op == 2) batchDeleteStations(network, found);
            }
            break;
        }
        case 10: {
            int from = inputInt("From CS ID: ");
            int to = inputInt("To CS ID: ");
            int d = inputInt("Diameter (500/700/1000/1400): ");
            network.connect(from, to, d);
            break;
        }
        case 11: {
            vector<int> topo = network.topologicalSort();
            if (topo.size() != network.getStations().size()) {
                cout << "Network has cycles! Topological sort not possible.\n";
            }
            else {
                cout << "Topological order: ";
                for (int id : topo) cout << id << " ";
                cout << "\n";
            }
            break;
        }
        case 12: {
            int src = inputInt("Source CS ID: ");
            int sink = inputInt("Sink CS ID: ");
            double flow = network.maxFlow(src, sink);
            cout << fixed << setprecision(3);  // ← 3 знака после запятой
            cout << "Max flow: " << flow << "\n";
            break;
        }
        case 13: {
            int from = inputInt("From CS ID: ");
            int to = inputInt("To CS ID: ");
            vector<int> path = network.shortestPath(from, to);
            if (path.size() <= 1) {
                cout << "No path found.\n";
            }
            else {
                cout << "Shortest path: ";
                for (size_t i = 0; i < path.size(); ++i) {
                    cout << path[i];
                    if (i + 1 < path.size()) cout << " -> ";
                }
                cout << "\n";
            }
            break;
        }
        case 14: {
            string fname = inputString("Enter filename to save: ");
            if (fname.empty()) fname = "data.txt";
            saveAll(network, fname);
            break;
        }
        case 15: {
            string fname = inputString("Enter filename to load: ");
            if (fname.empty()) fname = "data.txt";
            loadAll(network, fname);
            break;
        }
        case 16:
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid option.\n";
        }
    }
}