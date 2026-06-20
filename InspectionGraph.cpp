#include "InspectionGraph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Purpose: Initialize graph
InspectionGraph::InspectionGraph() {}

// Purpose: Destructor
InspectionGraph::~InspectionGraph() {}

// Purpose: Register a checkpoint vertex
void InspectionGraph::addInspectionPoint(const string& name) {
    if (adjList.find(name) == adjList.end()) {
        adjList[name] = vector<Edge>();
    }
}

// Purpose: Add a directed edge route
void InspectionGraph::addRoute(const string& source, const string& destination, double waitTime) {
    addInspectionPoint(source);
    addInspectionPoint(destination);
    adjList[source].push_back(Edge(destination, waitTime));
}

// Purpose: Print graph adjacency lists
void InspectionGraph::displayNetwork() const {
    if (adjList.empty()) {
        cout << "The customs inspection network is empty.\n";
        return;
    }

    cout << "\nCustoms Checkpoint Inspection Network (Adjacency List):\n";
    cout << string(60, '=') << "\n";

    for (const auto& pair : adjList) {
        cout << "Checkpoint [" << pair.first << "] connects to:\n";
        if (pair.second.empty()) {
            cout << "  └─ (No outgoing routes)\n";
        }
        for (const auto& edge : pair.second) {
            cout << "  ├──> [" << edge.destination << "] (Wait: " << edge.waitTime << " hrs)\n";
        }
    }
    cout << string(60, '=') << "\n";
}

// Purpose: Export routes to CSV
bool InspectionGraph::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "SourcePoint,DestinationPoint,WaitTime\n";
    for (const auto& pair : adjList) {
        string source = pair.first;
        for (const auto& edge : pair.second) {
            file << source << "," << edge.destination << "," << edge.waitTime << "\n";
        }
    }

    file.close();
    return true;
}

// Purpose: Import routes from CSV
bool InspectionGraph::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    clear();
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string src, dest, waitStr;

        getline(ss, src, ',');
        getline(ss, dest, ',');
        getline(ss, waitStr, ',');

        double wait = 0.0;
        try {
            if (!waitStr.empty()) {
                wait = stod(waitStr);
            }
        } catch (...) {}

        addRoute(src, dest, wait);
    }

    file.close();
    return true;
}

// Purpose: Gather all checkpoint labels
vector<string> InspectionGraph::getVertices() const {
    vector<string> vertices;
    for (const auto& pair : adjList) {
        vertices.push_back(pair.first);
    }
    return vertices;
}
