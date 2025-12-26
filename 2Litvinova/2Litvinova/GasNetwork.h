#pragma once
#include "Pipe.h"
#include "CompressorStation.h"
#include "Connection.h"
#include <unordered_map>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

class GasNetwork {
private:
    unordered_map<int, CompressorStation> stations;
    unordered_map<int, Pipe> pipes;
    vector<Connection> connections;
    map<int, vector<int>> free_pipes_by_diam;

    int next_pipe_id = 1;
    int next_station_id = 1;

    void updateFreePipes();

public:
    void addStation(const CompressorStation& cs);
    void addPipe(const Pipe& p);
    bool connect(int from_id, int to_id, int diam);
    vector<int> topologicalSort() const;
    void printNetwork() const;
    bool hasStation(int id) const { return stations.count(id); }
    const unordered_map<int, Pipe>& getPipes() const { return pipes; }
    const unordered_map<int, CompressorStation>& getStations() const { return stations; }
    void deletePipe(int pipe_id);
    void deleteStation(int station_id);

    double getCapacity(int pipe_id) const;
    double getWeight(int pipe_id) const;
    double maxFlow(int source, int sink) const;
    vector<int> shortestPath(int from, int to) const;
};