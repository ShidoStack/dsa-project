#ifndef TARIFF_TREE_H
#define TARIFF_TREE_H

#include <string>
#include <vector>

using namespace std;

// Classifying customs items in a general tree structure
struct TariffNode {
    string name;
    double dutyRate;
    vector<TariffNode*> children;

    TariffNode(string n, double rate = 0.0) : name(n), dutyRate(rate) {}

    ~TariffNode() {
        for (TariffNode* child : children) {
            delete child;
        }
    }
};

// General tree representing commodity codes and duty rates
class TariffTree {
private:
    TariffNode* root;

    TariffNode* findNode(TariffNode* current, const string& name) const;
    void displayHelper(TariffNode* node, int depth) const;
    void saveHelper(ofstream& file, TariffNode* node, const string& parentName) const;

public:
    TariffTree();
    ~TariffTree();

    // Core category operations
    bool addCategory(const string& parentName, const string& categoryName, double dutyRate = 0.0);
    TariffNode* searchCategory(const string& name) const;
    void displayTree() const;
    bool saveToCSV(const string& filename) const;
    bool loadFromCSV(const string& filename);

    // Look up duty rate for a specific category
    double getDutyRate(const string& categoryName) const;
    TariffNode* getRoot() const { return root; }
};

#endif // TARIFF_TREE_H
