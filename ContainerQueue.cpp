#include "ContainerQueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Purpose: Initialize queue
ContainerQueue::ContainerQueue() {}

// Purpose: Destructor
ContainerQueue::~ContainerQueue() {}

// Purpose: Enqueue a container at the end of the line
void ContainerQueue::enqueueContainer(const Container& container) {
    q.push(container);
}

// Purpose: Dequeue and process the container at the front
bool ContainerQueue::processNextContainer(Container& outContainer) {
    if (q.empty()) {
        return false;
    }
    outContainer = q.front();
    q.pop();
    return true;
}

// Purpose: Display all containers in the queue
void ContainerQueue::displayQueue() const {
    if (q.empty()) {
        cout << "No containers currently in the checking pipeline.\n";
        return;
    }

    cout << "\nChecking Pipeline (FIFO Queue):\n";
    cout << left << setw(12) << "ID" 
         << setw(20) << "Description" 
         << setw(10) << "Weight" 
         << setw(15) << "Category" 
         << setw(12) << "Value ($)" 
         << setw(12) << "Duty ($)" 
         << setw(15) << "Importer" 
         << setw(15) << "Status" << "\n";
    cout << string(113, '-') << "\n";

    queue<Container> tempQueue = q;
    while (!tempQueue.empty()) {
        Container c = tempQueue.front();
        cout << left << setw(12) << c.id
             << setw(20) << (c.cargoDesc.length() > 18 ? c.cargoDesc.substr(0, 15) + "..." : c.cargoDesc)
             << setw(10) << c.weight
             << setw(15) << c.tariffCategory
             << setw(12) << c.value
             << setw(12) << c.customsDuty
             << setw(15) << c.importerName
             << setw(15) << c.status << "\n";
        tempQueue.pop();
    }
    cout << string(113, '-') << "\n";
}

// Purpose: Save all queue items to a CSV file
bool ContainerQueue::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "ContainerID,CargoDescription,Weight,TariffCategory,Value,CustomsDuty,ImporterName,Status\n";
    queue<Container> tempQueue = q;
    while (!tempQueue.empty()) {
        Container c = tempQueue.front();
        file << c.id << ","
             << c.cargoDesc << ","
             << c.weight << ","
             << c.tariffCategory << ","
             << c.value << ","
             << c.customsDuty << ","
             << c.importerName << ","
             << c.status << "\n";
        tempQueue.pop();
    }

    file.close();
    return true;
}

// Purpose: Load queue items from a CSV file
bool ContainerQueue::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    clear();
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string cid, desc, weightStr, cat, valStr, dutyStr, importer, status;

        getline(ss, cid, ',');
        getline(ss, desc, ',');
        getline(ss, weightStr, ',');
        getline(ss, cat, ',');
        getline(ss, valStr, ',');
        getline(ss, dutyStr, ',');
        getline(ss, importer, ',');
        getline(ss, status, ',');

        double weight = 0.0, val = 0.0, duty = 0.0;
        try {
            if (!weightStr.empty()) weight = stod(weightStr);
            if (!valStr.empty()) val = stod(valStr);
            if (!dutyStr.empty()) duty = stod(dutyStr);
        } catch (...) {}

        enqueueContainer(Container(cid, desc, weight, cat, val, duty, importer, status));
    }

    file.close();
    return true;
}

// Purpose: Clear all items from queue
void ContainerQueue::clear() {
    while (!q.empty()) {
        q.pop();
    }
}

// Purpose: Fetch the front container details without removing it
bool ContainerQueue::getFront(Container& outContainer) const {
    if (q.empty()) return false;
    outContainer = q.front();
    return true;
}
