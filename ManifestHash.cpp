#include "ManifestHash.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Purpose: Initialize hash table
ManifestHash::ManifestHash() {}

// Purpose: Destructor
ManifestHash::~ManifestHash() {}

// Purpose: Compute a hash fingerprint of string document content using hash<string>
size_t ManifestHash::calculateHash(const string& text) const {
    hash<string> hashFn;
    return hashFn(text);
}

// Purpose: Register manifest ID and computed hash mapping
void ManifestHash::registerManifest(const string& manifestId, const string& docContent) {
    size_t hashVal = calculateHash(docContent);
    registry[manifestId] = hashVal;
}

// Purpose: Verify if document matches stored signature hash
bool ManifestHash::verifyManifest(const string& manifestId, const string& docContent) const {
    auto it = registry.find(manifestId);
    if (it == registry.end()) {
        cout << "[Verification Failed] Manifest ID '" << manifestId << "' is not registered.\n";
        return false;
    }
    
    size_t computedHash = calculateHash(docContent);
    if (it->second == computedHash) {
        return true;
    } else {
        cout << "[Verification Failed] Hash Mismatch! Content altered. Stored: " 
             << it->second << ", Computed: " << computedHash << "\n";
        return false;
    }
}

// Purpose: Display all registered manifest signatures
void ManifestHash::displayManifests() const {
    if (registry.empty()) {
        cout << "No manifests registered in the system.\n";
        return;
    }

    cout << "\nRegistered Cargo Manifests (Hash Table):\n";
    cout << left << setw(20) << "Manifest ID" << setw(25) << "Hash Signature (hash<string>)" << "\n";
    cout << string(45, '-') << "\n";
    
    for (const auto& pair : registry) {
        cout << left << setw(20) << pair.first
             << setw(25) << pair.second << "\n";
    }
    cout << string(45, '-') << "\n";
}

// Purpose: Save manifest registries to a CSV file
bool ManifestHash::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "ManifestID,HashValue\n";
    for (const auto& pair : registry) {
        file << pair.first << "," << pair.second << "\n";
    }

    file.close();
    return true;
}

// Purpose: Load manifest registries from a CSV file
bool ManifestHash::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    registry.clear();
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string manifestId, hashValStr;

        getline(ss, manifestId, ',');
        getline(ss, hashValStr, ',');

        size_t hashVal = 0;
        try {
            if (!hashValStr.empty()) {
                hashVal = stoull(hashValStr);
            }
        } catch (...) {}

        registerDirectHash(manifestId, hashVal);
    }

    file.close();
    return true;
}

// Purpose: Directly store pre-computed hash mapping
void ManifestHash::registerDirectHash(const string& manifestId, size_t hashValue) {
    registry[manifestId] = hashValue;
}
