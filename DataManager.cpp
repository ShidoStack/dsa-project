#include "DataManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Purpose: Constructor
DataManager::DataManager() {}

// Purpose: Destructor
DataManager::~DataManager() {}

// Purpose: Check if a file exists on disk
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Purpose: Seed default database tables with expanded logistics catalog
void DataManager::initializeDefaultData() {
    // Seed new classification tree
    if (!fileExists("categories.csv")) {
        ofstream f("categories.csv");
        if (f.is_open()) {
            f << "CategoryName,ParentName,DutyRate\n"
              << "Products,Root,0.0\n"
              << "Electronics,Products,0.0\n"
              << "Laptop,Electronics,0.0\n"
              << "Gaming Laptop,Laptop,15.0\n"
              << "Business Laptop,Laptop,10.0\n"
              << "Student Laptop,Laptop,5.0\n"
              << "Mobile,Electronics,0.0\n"
              << "Android,Mobile,8.0\n"
              << "iPhone,Mobile,10.0\n"
              << "Feature Phone,Mobile,4.0\n"
              << "Tablet,Electronics,6.0\n"
              << "Smart Watch,Electronics,5.0\n"
              << "Food,Products,0.0\n"
              << "Fruits,Food,0.0\n"
              << "Apple,Fruits,4.0\n"
              << "Mango,Fruits,5.0\n"
              << "Banana,Fruits,3.0\n"
              << "Vegetables,Food,4.5\n"
              << "Dairy,Food,7.5\n"
              << "Beverages,Food,8.0\n"
              << "Clothing,Products,0.0\n"
              << "Men,Clothing,0.0\n"
              << "Shirts,Men,10.0\n"
              << "Jeans,Men,12.0\n"
              << "Jackets,Men,15.0\n"
              << "Women,Clothing,12.0\n"
              << "Kids,Clothing,8.0\n"
              << "Industrial Goods,Products,0.0\n"
              << "Machinery,Industrial Goods,18.0\n"
              << "Tools,Industrial Goods,12.0\n"
              << "Safety Equipment,Industrial Goods,5.0\n"
              << "Medical Supplies,Products,0.0\n"
              << "Masks,Medical Supplies,2.0\n"
              << "Gloves,Medical Supplies,2.0\n"
              << "First Aid,Medical Supplies,4.0\n";
            f.close();
        }
    }

    // Seed checkpoint network routes
    if (!fileExists("routes.csv")) {
        ofstream f("routes.csv");
        if (f.is_open()) {
            f << "SourcePoint,DestinationPoint,WaitTime\n"
              << "Mumbai_Port,Dubai_Port,1.5\n"
              << "Dubai_Port,Singapore_Port,2.0\n"
              << "Dubai_Port,Rotterdam_Port,4.0\n"
              << "Rotterdam_Port,Singapore_Port,3.0\n"
              << "Singapore_Port,Hamburg_Port,0.5\n";
            f.close();
        }
    }

    // Seed default inspectors
    if (!fileExists("inspectors.csv")) {
        ofstream f("inspectors.csv");
        if (f.is_open()) {
            f << "InspectorName,Workload\n"
              << "Officer_Sarah,3\n"
              << "Officer_Raj,1\n"
              << "Officer_Elena,0\n"
              << "Officer_Kofi,2\n";
            f.close();
        }
    }

    // Seed arrival queue matching new classification items
    if (!fileExists("containers.csv")) {
        ofstream f("containers.csv");
        if (f.is_open()) {
            f << "ContainerID,CargoDescription,Weight,TariffCategory,Value,CustomsDuty,ImporterName,Status\n"
              << "CONT-101,iPhone shipment,2.4,iPhone,150000.0,15000.0,Apple_Retail,Pending\n"
              << "CONT-102,Fresh Apples,12.0,Apple,20000.0,800.0,FruitImp_LLC,Pending\n"
              << "CONT-103,Industrial CNC Mill,2.5,Machinery,100000.0,18000.0,Industrial_Imports,Pending\n"
              << "CONT-104,Precision Toolsets,1.2,Tools,30000.0,3600.0,ToolPro_Corp,Pending\n";
            f.close();
        }
    }

    // Seed manifests matching new container descriptions and values
    if (!fileExists("manifests.csv")) {
        ManifestHash tempHash;
        tempHash.registerManifest("CONT-101", "Apple Inc. Cargo Manifest: 500x iPhones, weight 2.4 tons, value $150000");
        tempHash.registerManifest("CONT-102", "Fruit Importers LLC Cargo Manifest: 5000x Fresh Apples, weight 12.0 tons, value $20000");
        tempHash.registerManifest("CONT-103", "Industrial Imports Cargo Manifest: 1x Industrial CNC Mill, weight 2.5 tons, value $100000");
        tempHash.registerManifest("CONT-104", "ToolPro Corp Cargo Manifest: 150x Precision Toolsets, weight 1.2 tons, value $30000");
        tempHash.saveToCSV("manifests.csv");
    }

    // Seed shipments ledger
    if (!fileExists("shipments.csv")) {
        ofstream f("shipments.csv");
        if (f.is_open()) {
            f << "ShipmentID,Description,Category,Value,CustomsDuty,ImporterName\n"
              << "CONT-099,Student Laptop batch,Student Laptop,80000.0,4000.0,Dell_Direct\n"
              << "CONT-098,Cheddar Cheese,Dairy,15000.0,1125.0,Global_Foods\n";
            f.close();
        }
    }

    // Seed validation history logs
    if (!fileExists("validations.csv")) {
        ofstream f("validations.csv");
        if (f.is_open()) {
            f << "ContainerID,StepDescription,Status,Timestamp\n"
              << "CONT-099,Hamburg_Port,Passed,2026-06-18_08:00\n"
              << "CONT-098,Singapore_Port,Passed,2026-06-18_08:15\n";
            f.close();
        }
    }
}

// Purpose: Read data files into memory models
bool DataManager::loadAllData(TariffTree& tree,
                            ValidationStack& stack,
                            ContainerQueue& queue,
                            ManifestHash& manifestHash,
                            ShipmentSorter& sorter,
                            InspectionGraph& graph,
                            InspectorScheduler& scheduler) {
    initializeDefaultData();

    bool ok = true;
    ok &= tree.loadFromCSV("categories.csv");
    ok &= stack.loadFromCSV("validations.csv");
    ok &= queue.loadFromCSV("containers.csv");
    ok &= manifestHash.loadFromCSV("manifests.csv");
    ok &= sorter.loadFromCSV("shipments.csv");
    ok &= graph.loadFromCSV("routes.csv");
    ok &= scheduler.loadFromCSV("inspectors.csv");

    return ok;
}

// Purpose: Write memory models states back to CSV files
bool DataManager::saveAllData(const TariffTree& tree,
                            const ValidationStack& stack,
                            const ContainerQueue& queue,
                            const ManifestHash& manifestHash,
                            const ShipmentSorter& sorter,
                            const InspectionGraph& graph,
                            const InspectorScheduler& scheduler) {
    bool ok = true;
    ok &= tree.saveToCSV("categories.csv");
    ok &= stack.saveToCSV("validations.csv");
    ok &= queue.saveToCSV("containers.csv");
    ok &= manifestHash.saveToCSV("manifests.csv");
    ok &= sorter.saveToCSV("shipments.csv");
    ok &= graph.saveToCSV("routes.csv");
    ok &= scheduler.saveToCSV("inspectors.csv");

    return ok;
}
