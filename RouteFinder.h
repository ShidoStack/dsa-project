#ifndef ROUTE_FINDER_H
#define ROUTE_FINDER_H

#include "InspectionGraph.h"
#include <string>
#include <vector>

using namespace std;

// Dijkstra shortest path computation utility
class RouteFinder {
public:
    RouteFinder();
    ~RouteFinder();

    // Shortest path algorithms
    static bool findShortestRoute(const InspectionGraph& graph, 
                                  const string& source, 
                                  const string& destination, 
                                  vector<string>& outPath, 
                                  double& outTotalWait);

    static void displayRoute(const vector<string>& path, double totalWait);
};

#endif // ROUTE_FINDER_H
