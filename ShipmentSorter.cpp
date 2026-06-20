#include "ShipmentSorter.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Purpose: Initialize sorter
ShipmentSorter::ShipmentSorter() {}

// Purpose: Destructor
ShipmentSorter::~ShipmentSorter() {}

// Purpose: Add a shipment to the collection
void ShipmentSorter::addShipment(const Shipment& shipment) {
    shipments.push_back(shipment);
}

// Purpose: Sort shipments by duty in ascending order
void ShipmentSorter::sortAscending() {
    sort(shipments.begin(), shipments.end(), [](const Shipment& a, const Shipment& b) {
        return a.duty < b.duty;
    });
}

// Purpose: Sort shipments by duty in descending order
void ShipmentSorter::sortDescending() {
    sort(shipments.begin(), shipments.end(), [](const Shipment& a, const Shipment& b) {
        return a.duty > b.duty;
    });
}

// Purpose: Print shipment records in a ledger format
void ShipmentSorter::displayShipments() const {
    if (shipments.empty()) {
        cout << "No shipment records available.\n";
        return;
    }

    cout << "\nShipment Records:\n";
    cout << left << setw(12) << "ID" 
         << setw(20) << "Description" 
         << setw(15) << "Category" 
         << setw(12) << "Value ($)" 
         << setw(12) << "Duty ($)" 
         << setw(20) << "Importer Name" << "\n";
    cout << string(91, '-') << "\n";

    for (const auto& s : shipments) {
        cout << left << setw(12) << s.id
             << setw(20) << (s.description.length() > 18 ? s.description.substr(0, 15) + "..." : s.description)
             << setw(15) << s.category
             << setw(12) << s.value
             << setw(12) << s.duty
             << setw(20) << s.importerName << "\n";
    }
    cout << string(91, '-') << "\n";
}

// Purpose: Save shipments ledger to a CSV file
bool ShipmentSorter::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "ShipmentID,Description,Category,Value,CustomsDuty,ImporterName\n";
    for (const auto& s : shipments) {
        file << s.id << ","
             << s.description << ","
             << s.category << ","
             << s.value << ","
             << s.duty << ","
             << s.importerName << "\n";
    }

    file.close();
    return true;
}

// Purpose: Load shipments ledger from a CSV file
bool ShipmentSorter::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    shipments.clear();
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sid, desc, cat, valStr, dutyStr, importer;

        getline(ss, sid, ',');
        getline(ss, desc, ',');
        getline(ss, cat, ',');
        getline(ss, valStr, ',');
        getline(ss, dutyStr, ',');
        getline(ss, importer, ',');

        double val = 0.0, duty = 0.0;
        try {
            if (!valStr.empty()) val = stod(valStr);
            if (!dutyStr.empty()) duty = stod(dutyStr);
        } catch (...) {}

        addShipment(Shipment(sid, desc, cat, val, duty, importer));
    }

    file.close();
    return true;
}
