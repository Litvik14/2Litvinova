#include "Logger.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;

static string logFilename = "pipeline.log";

void Logger::setLogFile(const string& filename) {
    logFilename = filename;
}

void Logger::log(const string& message) {
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    auto ms = chrono::duration_cast<chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    ostringstream oss;
    oss << "[" << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S")
        << "." << setfill('0') << setw(3) << ms.count()
        << "] " << message;

    string fullMessage = oss.str();

    ofstream logFile(logFilename, ios::app);
    if (logFile) {
        logFile << fullMessage << "\n";
        logFile.close();
    }

    cout << "[LOG] " << message << "\n";
}