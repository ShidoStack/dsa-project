#include "ValidationStack.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Purpose: Initialize validation stack
ValidationStack::ValidationStack() {}

// Purpose: Destructor
ValidationStack::~ValidationStack() {}

// Purpose: Add a new validation step onto the stack
void ValidationStack::addValidationStep(const string& containerId, const string& stepDescription, const string& status, const string& timestamp) {
    stack.push_back(ValidationStep(containerId, stepDescription, status, timestamp));
}

// Purpose: Revert the last logged validation step
bool ValidationStack::undoLastValidation() {
    if (stack.empty()) {
        return false;
    }
    stack.pop_back();
    return true;
}

// Purpose: Inspect the most recent validation step
bool ValidationStack::viewLatestStep(ValidationStep& outStep) const {
    if (stack.empty()) {
        return false;
    }
    outStep = stack.back();
    return true;
}

// Purpose: Render validation history in a formatted table layout
void ValidationStack::displayAllSteps() const {
    if (stack.empty()) {
        cout << "No validation steps logged.\n";
        return;
    }

    cout << "\n## " << left << setw(15) << "Container ID" 
         << setw(35) << "Step Description" 
         << setw(15) << "Status" 
         << setw(20) << "Timestamp" << "\n\n";

    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
        string displayStatus = it->status;
        if (displayStatus == "Passed") displayStatus = "[PASS]";
        else if (displayStatus == "Failed") displayStatus = "[FAIL]";

        cout << left << setw(18) << it->containerId
             << setw(35) << it->stepDescription
             << setw(15) << displayStatus
             << setw(20) << it->timestamp << "\n";
    }
}

// Purpose: Write validation history records to a CSV file
bool ValidationStack::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "ContainerID,StepDescription,Status,Timestamp\n";
    for (const auto& step : stack) {
        file << step.containerId << ","
             << step.stepDescription << ","
             << step.status << ","
             << step.timestamp << "\n";
    }

    file.close();
    return true;
}

// Purpose: Read validation history records from a CSV file
bool ValidationStack::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    stack.clear();
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string cid, desc, stat, time;

        getline(ss, cid, ',');
        getline(ss, desc, ',');
        getline(ss, stat, ',');
        getline(ss, time, ',');

        addValidationStep(cid, desc, stat, time);
    }

    file.close();
    return true;
}
