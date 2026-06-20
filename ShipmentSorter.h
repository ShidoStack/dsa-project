#ifndef SHIPMENT_SORTER_H
#define SHIPMENT_SORTER_H

#include <string>
#include <vector>

using namespace std;

// Struct containing cargo shipment details
struct Shipment {
    string id;
    string description;
    string category;
    double value;
    double duty;
    string importerName;

    Shipment(string sid, string desc, string cat, double val, double dt, string importer)
        : id(sid), description(desc), category(cat), value(val), duty(dt), importerName(importer) {}
};

// Sorter system to organize records by customs duties
class ShipmentSorter {
private:
    vector<Shipment> shipments;

public:
    ShipmentSorter();
    ~ShipmentSorter();

    // Core sorting operations
    void addShipment(const Shipment& shipment);
    void sortAscending();
    void sortDescending();
    void displayShipments() const;
    bool saveToCSV(const string& filename) const;
    bool loadFromCSV(const string& filename);

    void clear() { shipments.clear(); }
    size_t size() const { return shipments.size(); }
    const vector<Shipment>& getShipments() const { return shipments; }
};

#endif // SHIPMENT_SORTER_H
