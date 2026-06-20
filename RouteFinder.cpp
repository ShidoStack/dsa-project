#include "RouteFinder.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <iostream>

using namespace std;

// Purpose: Constructor
RouteFinder::RouteFinder() {}

// Purpose: Destructor
RouteFinder::~RouteFinder() {}

// Purpose: Execute Dijkstra's shortest path search algorithm
bool RouteFinder::findShortestRoute(const InspectionGraph& graph, 
                                    const string& source, 
                                    const string& destination, 
                                    vector<string>& outPath, 
                                    double& outTotalWait) {
    outPath.clear();
    outTotalWait = 0.0;

    if (!graph.hasVertex(source) || !graph.hasVertex(destination)) {
        return false;
    }

    const auto& adjList = graph.getAdjacencyList();
    unordered_map<string, double> dist;
    unordered_map<string, string> parent;

    for (const auto& pair : adjList) {
        dist[pair.first] = numeric_limits<double>::max();
    }

    dist[source] = 0.0;

    typedef pair<double, string> PQElement;
    priority_queue<PQElement, vector<PQElement>, greater<PQElement>> pq;

    pq.push({0.0, source});

    while (!pq.empty()) {
        double d = pq.top().first;
        string u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == destination) break;

        auto it = adjList.find(u);
        if (it == adjList.end()) continue;

        for (const auto& edge : it->second) {
            string v = edge.destination;
            double weight = edge.waitTime;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[destination] == numeric_limits<double>::max()) {
        return false;
    }

    string current = destination;
    while (current != source) {
        outPath.push_back(current);
        auto pIt = parent.find(current);
        if (pIt == parent.end()) {
            return false;
        }
        current = pIt->second;
    }
    outPath.push_back(source);

    reverse(outPath.begin(), outPath.end());
    outTotalWait = dist[destination];
    return true;
}

// Purpose: Render shortest route details
void RouteFinder::displayRoute(const vector<string>& path, double totalWait) {
    if (path.empty()) {
        cout << "No path details to display.\n";
        return;
    }

    cout << "\n--- Route Calculation ---\n";
    cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << "[" << path[i] << "]";
        if (i < path.size() - 1) {
            cout << " ──> ";
        }
    }
    cout << "\nTotal Clearance/Transit Wait Time: " << totalWait << " hours\n";
    cout << "-------------------------\n";
}
