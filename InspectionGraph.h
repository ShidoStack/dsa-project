#ifndef INSPECTION_GRAPH_H
#define INSPECTION_GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Struct containing route edge properties
struct Edge {
    string destination;
    double waitTime; 

    Edge(string dest, double wait) : destination(dest), waitTime(wait) {}
};

// Adjacency list representation of border inspection checkpoints
class InspectionGraph {
private:
    unordered_map<string, vector<Edge>> adjList;

public:
    InspectionGraph();
    ~InspectionGraph();

    // Core graph operations
    void addInspectionPoint(const string& name);
    void addRoute(const string& source, const string& destination, double waitTime);
    void displayNetwork() const;
    bool saveToCSV(const string& filename) const;
    bool loadFromCSV(const string& filename);

    void clear() { adjList.clear(); }
    const unordered_map<string, vector<Edge>>& getAdjacencyList() const { return adjList; }
    bool hasVertex(const string& name) const { return adjList.find(name) != adjList.end(); }
    vector<string> getVertices() const;
};

#endif // INSPECTION_GRAPH_H
