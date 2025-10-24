#pragma once
#include <map>
#include <vector>
#include <string>
#include "Pipe.h"
#include "CompressorStation.h"

using namespace std;

class Database {
private:
    map<int, Pipe> pipes;
    map<int, CompressorStation> stations;
    int nextPipeId = 1;
    int nextStationId = 1;

public:
    int addPipe();
    void editPipe(int id);
    void deletePipe(int id);
    vector<int> findPipesByName(const string& name);
    vector<int> findPipesByStatus(int status);
    void printAllPipes() const;
    void savePipes(const string& filename) const;
    void loadPipes(const string& filename);

    int addStation();
    void editStation(int id);
    void deleteStation(int id);
    vector<int> findStationsByName(const string& name);
    vector<int> findStationsByIdlePercent(float minIdle, float maxIdle);
    void printAllStations() const;
    void saveStations(const string& filename) const;
    void loadStations(const string& filename);

    void batchEditPipes(const vector<int>& ids);
    void batchDeletePipes(const vector<int>& ids);
};