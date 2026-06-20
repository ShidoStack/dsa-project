#include "InspectorScheduler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Purpose: Constructor
InspectorScheduler::InspectorScheduler() {}

// Purpose: Destructor
InspectorScheduler::~InspectorScheduler() {}

// Purpose: Register a new inspector to the scheduler
void InspectorScheduler::addInspector(const string& name, int workload) {
    pq.push(Inspector(name, workload));
}

// Purpose: Route a container task to the inspector with the lowest workload
bool InspectorScheduler::assignContainer(const string& containerId, string& outInspectorName) {
    if (pq.empty()) {
        cout << "[Scheduling Error] Cannot assign container " << containerId 
             << ". No inspectors registered in the system.\n";
        return false;
    }

    Inspector leastBusy = pq.top();
    pq.pop();

    outInspectorName = leastBusy.name;
    leastBusy.workload++;

    pq.push(leastBusy);

    cout << "[Scheduler] Container " << containerId << " assigned to Inspector " 
         << outInspectorName << " (New workload: " << leastBusy.workload << " tasks).\n";
    return true;
}

// Purpose: Display inspectors by workload order
void InspectorScheduler::displayInspectorWorkloads() const {
    if (pq.empty()) {
        cout << "No inspectors registered in the scheduler.\n";
        return;
    }

    cout << "\nInspector Workloads (Min-Heap Sorted):\n";
    cout << left << setw(20) << "Inspector Name" << setw(15) << "Workload (Tasks)" << "\n";
    cout << string(35, '-') << "\n";

    auto tempPq = pq;
    while (!tempPq.empty()) {
        Inspector ins = tempPq.top();
        cout << left << setw(20) << ins.name
             << setw(15) << ins.workload << "\n";
        tempPq.pop();
    }
    cout << string(35, '-') << "\n";
}

// Purpose: Export inspectors to a CSV file
bool InspectorScheduler::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "InspectorName,Workload\n";

    auto tempPq = pq;
    while (!tempPq.empty()) {
        Inspector ins = tempPq.top();
        file << ins.name << "," << ins.workload << "\n";
        tempPq.pop();
    }

    file.close();
    return true;
}

// Purpose: Import inspectors from a CSV file
bool InspectorScheduler::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    clear();
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, workloadStr;

        getline(ss, name, ',');
        getline(ss, workloadStr, ',');

        int workload = 0;
        try {
            if (!workloadStr.empty()) {
                workload = stoi(workloadStr);
            }
        } catch (...) {}

        addInspector(name, workload);
    }

    file.close();
    return true;
}

// Purpose: Clear inspector database
void InspectorScheduler::clear() {
    while (!pq.empty()) {
        pq.pop();
    }
}

// Purpose: Extract inspector list into a vector
vector<Inspector> InspectorScheduler::getInspectorsList() const {
    vector<Inspector> list;
    auto tempPq = pq;
    while (!tempPq.empty()) {
        list.push_back(tempPq.top());
        tempPq.pop();
    }
    return list;
}
