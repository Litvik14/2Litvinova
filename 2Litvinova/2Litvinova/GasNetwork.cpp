#include "GasNetwork.h"
#include "utils.h"
#include <iostream>
#include <set>
#include <cmath>
#include <limits>
#include <queue>
#include <functional>
using namespace std;

const double INF = 1e9;

void GasNetwork::updateFreePipes() {
    free_pipes_by_diam.clear();
    set<int> allowed = { 500, 700, 1000, 1400 };
    for (const auto& item : pipes) {
        const Pipe& p = item.second;
        if (allowed.find(p.getDiametr()) != allowed.end()) {
            bool used = false;
            for (const auto& conn : connections) {
                if (conn.getPipeId() == p.getId()) {
                    used = true;
                    break;
                }
            }
            if (!used) {
                free_pipes_by_diam[p.getDiametr()].push_back(p.getId());
            }
        }
    }
}

void GasNetwork::addStation(const CompressorStation& cs) {
    if (cs.getId() == -1) {
        CompressorStation s(next_station_id++);
        s.set(cs.getName(), cs.getNumberWork(), cs.getNumberWorkOnline(), cs.getClass());
        stations[s.getId()] = s;
    }
    else {
        stations[cs.getId()] = cs;
        if (cs.getId() >= next_station_id) next_station_id = cs.getId() + 1;
    }
}

void GasNetwork::addPipe(const Pipe& p) {
    if (p.getId() == -1) {
        Pipe new_pipe(next_pipe_id++);
        new_pipe.set(p.getName(), p.getLength(), p.getDiametr(), p.getStatus());
        pipes[new_pipe.getId()] = new_pipe;
    }
    else {
        pipes[p.getId()] = p;
        if (p.getId() >= next_pipe_id) next_pipe_id = p.getId() + 1;
    }
    updateFreePipes();
}

bool GasNetwork::connect(int from_id, int to_id, int diam) {
    if (!stations.count(from_id) || !stations.count(to_id)) {
        cout << "Error: Station not found.\n";
        return false;
    }

    set<int> allowed = { 500, 700, 1000, 1400 };
    if (allowed.find(diam) == allowed.end()) {
        cout << "Invalid diameter.\n";
        return false;
    }

    if (!free_pipes_by_diam[diam].empty()) {
        cout << "Found " << free_pipes_by_diam[diam].size()
            << " available pipe(s) with diameter " << diam << ".\n";
        if (confirm("Use existing pipe?")) {
            int pipe_id = free_pipes_by_diam[diam].back();
            free_pipes_by_diam[diam].pop_back();
            connections.emplace_back(from_id, to_id, pipe_id);
            cout << "Connected using existing pipe ID=" << pipe_id << "\n";
            return true;
        }
    }

    Pipe p(next_pipe_id++);
    string name = "AutoPipe_" + to_string(from_id) + "_" + to_string(to_id);
    p.set(name, 100.0f, diam, 0);
    pipes[p.getId()] = p;
    connections.emplace_back(from_id, to_id, p.getId());
    cout << "Created new pipe ID=" << p.getId() << "\n";
    updateFreePipes();
    return true;
}

vector<int> GasNetwork::topologicalSort() const {
    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> in_degree;
    set<int> all_nodes;

    for (const auto& item : stations) {
        all_nodes.insert(item.first);
        in_degree[item.first] = 0;
    }

    for (const auto& conn : connections) {
        int u = conn.getFrom();
        int v = conn.getTo();
        if (all_nodes.count(u) && all_nodes.count(v)) {
            graph[u].push_back(v);
            in_degree[v]++;
        }
    }

    queue<int> q;
    for (int node : all_nodes) {
        if (in_degree[node] == 0) {
            q.push(node);
        }
    }

    vector<int> result;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        result.push_back(u);
        for (int v : graph[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    return result;
}

void GasNetwork::printNetwork() const {
    if (connections.empty()) {
        cout << "\nNo connections.\n";
        return;
    }
    cout << "\n--- CONNECTIONS ---\n";
    for (const auto& conn : connections) {
        const Pipe& p = pipes.at(conn.getPipeId());
        cout << "CS " << conn.getFrom() << " --[Pipe " << p.getId() << " (d=" << p.getDiametr() << ")]--> CS " << conn.getTo() << "\n";
    }
}

void GasNetwork::deletePipe(int pipe_id) {
    if (!pipes.count(pipe_id)) return;
    pipes.erase(pipe_id);
    connections.erase(
        remove_if(connections.begin(), connections.end(),
            [pipe_id](const Connection& c) { return c.getPipeId() == pipe_id; }),
        connections.end()
    );
    updateFreePipes();
}

void GasNetwork::deleteStation(int station_id) {
    if (!stations.count(station_id)) return;

    set<int> pipes_to_delete;
    for (const auto& conn : connections) {
        if (conn.getFrom() == station_id || conn.getTo() == station_id) {
            pipes_to_delete.insert(conn.getPipeId());
        }
    }

    stations.erase(station_id);

    connections.erase(
        remove_if(connections.begin(), connections.end(),
            [station_id](const Connection& c) {
                return c.getFrom() == station_id || c.getTo() == station_id;
            }),
        connections.end()
    );

    for (int pipe_id : pipes_to_delete) {
        pipes.erase(pipe_id);
    }

    updateFreePipes();
}

double GasNetwork::getCapacity(int pipe_id) const {
    if (!pipes.count(pipe_id)) return 0.0;
    const Pipe& p = pipes.at(pipe_id);
    if (p.getStatus() == 1) return 0.0; 
    double d = p.getDiametr();
    double l = p.getLength();
    if (l <= 0) l = 1.0;
    return sqrt(pow(d, 5) / l) * 10000;
}

double GasNetwork::getWeight(int pipe_id) const {
    if (!pipes.count(pipe_id)) return INF;
    const Pipe& p = pipes.at(pipe_id);
    if (p.getStatus() == 1) return INF;
    return p.getLength();
}

double GasNetwork::maxFlow(int source, int sink) const {
    unordered_map<int, unordered_map<int, double>> residual;
    for (const auto& conn : connections) {
        int u = conn.getFrom();
        int v = conn.getTo();
        int pid = conn.getPipeId();
        double cap = getCapacity(pid);
        residual[u][v] += cap;
        residual[v][u] += cap; 
    }

    double total_flow = 0;
    vector<int> parent(stations.size() + 10);

    auto bfs = [&](int src, int snk) -> bool {
        fill(parent.begin(), parent.end(), -1);
        queue<int> q;
        q.push(src);
        parent[src] = -2;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& v : residual[u]) {
                if (parent[v.first] == -1 && v.second > 0) {
                    parent[v.first] = u;
                    if (v.first == snk) return true;
                    q.push(v.first);
                }
            }
        }
        return false;
        };

    while (bfs(source, sink)) {
        double flow = INF;
        int cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            flow = min(flow, residual[prev][cur]);
            cur = prev;
        }
        cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            residual[prev][cur] -= flow;
            residual[cur][prev] += flow;
            cur = prev;
        }
        total_flow += flow;
    }

    return total_flow;
}

vector<int> GasNetwork::shortestPath(int from, int to) const {
    unordered_map<int, unordered_map<int, double>> graph;
    for (const auto& conn : connections) {
        int u = conn.getFrom();
        int v = conn.getTo();
        int pid = conn.getPipeId();
        double w = getWeight(pid);
        graph[u][v] = w;
    }

    unordered_map<int, double> dist;
    unordered_map<int, int> parent;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

    for (const auto& st : stations) dist[st.first] = INF;
    dist[from] = 0;
    pq.push({ 0, from });

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (u == to) break;
        if (d > dist[u]) continue;

        for (auto& [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({ dist[v], v });
            }
        }
    }

    vector<int> path;
    int current = to;
    while (current != from) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(from);
    reverse(path.begin(), path.end());
    return path;
}