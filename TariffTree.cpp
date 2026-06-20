#include "TariffTree.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Purpose: Initialize the root node of the tree
TariffTree::TariffTree() {
    root = new TariffNode("Root", 0.0);
}

// Purpose: Recursively delete all nodes to prevent memory leaks
TariffTree::~TariffTree() {
    delete root;
}

// Purpose: Find a node by name in the subtree using DFS search
TariffNode* TariffTree::findNode(TariffNode* current, const string& name) const {
    if (!current) return nullptr;
    if (current->name == name) return current;

    for (TariffNode* child : current->children) {
        TariffNode* found = findNode(child, name);
        if (found) return found;
    }
    return nullptr;
}

// Purpose: Add a child category under the specified parent
bool TariffTree::addCategory(const string& parentName, const string& categoryName, double dutyRate) {
    if (findNode(root, categoryName) != nullptr) {
        return false;
    }

    TariffNode* parent = nullptr;
    if (parentName.empty() || parentName == "Root") {
        parent = root;
    } else {
        parent = findNode(root, parentName);
    }

    if (parent != nullptr) {
        parent->children.push_back(new TariffNode(categoryName, dutyRate));
        return true;
    }
    return false;
}

// Purpose: Public API to search for a category in the tree
TariffNode* TariffTree::searchCategory(const string& name) const {
    if (name == "Root") return root;
    return findNode(root, name);
}

// Purpose: Print tree categories recursively with indentation
void TariffTree::displayHelper(TariffNode* node, int depth) const {
    if (!node) return;

    if (node != root) {
        for (int i = 0; i < depth - 1; ++i) {
            cout << "│   ";
        }
        cout << "├── " << node->name;
        if (node->dutyRate > 0.0) {
            cout << " (Duty: " << node->dutyRate << "%)";
        }
        cout << "\n";
    }

    for (TariffNode* child : node->children) {
        displayHelper(child, depth + (node == root ? 0 : 1));
    }
}

// Purpose: Render the entire classification tree structure
void TariffTree::displayTree() const {
    cout << "Tariff Classification Hierarchy:\n";
    cout << "================================\n";
    displayHelper(root, 1);
    cout << "================================\n";
}

// Purpose: Save the tree relationship recursively in pre-order
void TariffTree::saveHelper(ofstream& file, TariffNode* node, const string& parentName) const {
    if (!node) return;

    if (node != root) {
        file << node->name << "," << parentName << "," << node->dutyRate << "\n";
    }

    for (TariffNode* child : node->children) {
        saveHelper(file, child, node->name);
    }
}

// Purpose: Export tree entries to a CSV file
bool TariffTree::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    file << "CategoryName,ParentName,DutyRate\n";
    saveHelper(file, root, "Root");
    file.close();
    return true;
}

// Purpose: Import tree layout from a CSV file
bool TariffTree::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    for (TariffNode* child : root->children) {
        delete child;
    }
    root->children.clear();

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string name, parent, rateStr;

        getline(ss, name, ',');
        getline(ss, parent, ',');
        getline(ss, rateStr, ',');

        double rate = 0.0;
        try {
            if (!rateStr.empty()) {
                rate = stod(rateStr);
            }
        } catch (...) {
            rate = 0.0;
        }

        addCategory(parent, name, rate);
    }

    file.close();
    return true;
}

// Purpose: Fetch the duty rate associated with a category
double TariffTree::getDutyRate(const string& categoryName) const {
    TariffNode* node = searchCategory(categoryName);
    return node ? node->dutyRate : 0.0;
}
