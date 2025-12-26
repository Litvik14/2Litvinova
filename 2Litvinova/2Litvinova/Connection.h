#pragma once
using namespace std;

class Connection {
private:
    int from_cs_id;
    int to_cs_id;
    int pipe_id;

public:
    Connection(int from, int to, int pipe) : from_cs_id(from), to_cs_id(to), pipe_id(pipe) {}
    int getFrom() const { return from_cs_id; }
    int getTo() const { return to_cs_id; }
    int getPipeId() const { return pipe_id; }
};