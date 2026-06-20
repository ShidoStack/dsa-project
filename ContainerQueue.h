#ifndef CONTAINER_QUEUE_H
#define CONTAINER_QUEUE_H

#include <string>
#include <queue>

using namespace std;

// Struct containing customs container metadata
struct Container {
    string id;
    string cargoDesc;
    double weight; 
    string tariffCategory;
    double value; 
    double customsDuty; 
    string importerName;
    string status; 

    Container() : weight(0.0), value(0.0), customsDuty(0.0) {}
    Container(string cid, string desc, double w, string cat, double val, double duty, string importer, string stat)
        : id(cid), cargoDesc(desc), weight(w), tariffCategory(cat), value(val), customsDuty(duty), importerName(importer), status(stat) {}
};

// Queue wrapper for FIFO checking pipeline
class ContainerQueue {
private:
    queue<Container> q;

public:
    ContainerQueue();
    ~ContainerQueue();

    // Core queue operations
    void enqueueContainer(const Container& container);
    bool processNextContainer(Container& outContainer);
    void displayQueue() const;
    bool saveToCSV(const string& filename) const;
    bool loadFromCSV(const string& filename);

    bool isEmpty() const { return q.empty(); }
    size_t size() const { return q.size(); }
    void clear();
    bool getFront(Container& outContainer) const;
};

#endif // CONTAINER_QUEUE_H
