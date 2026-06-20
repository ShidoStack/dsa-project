#ifndef INSPECTOR_SCHEDULER_H
#define INSPECTOR_SCHEDULER_H

#include <string>
#include <queue>
#include <vector>

using namespace std;

// Struct containing inspector profile and assignment workload
struct Inspector {
    string name;
    int workload; 

    Inspector(string n, int w = 0) : name(n), workload(w) {}

    bool operator>(const Inspector& other) const {
        if (workload == other.workload) {
            return name > other.name; 
        }
        return workload > other.workload;
    }
};

// Scheduler system balancing workload using a Min-Heap
class InspectorScheduler {
private:
    priority_queue<Inspector, vector<Inspector>, greater<Inspector>> pq;

public:
    InspectorScheduler();
    ~InspectorScheduler();

    // Core scheduler operations
    void addInspector(const string& name, int workload = 0);
    bool assignContainer(const string& containerId, string& outInspectorName);
    void displayInspectorWorkloads() const;
    bool saveToCSV(const string& filename) const;
    bool loadFromCSV(const string& filename);

    void clear();
    size_t size() const { return pq.size(); }
    vector<Inspector> getInspectorsList() const;
};

#endif // INSPECTOR_SCHEDULER_H
