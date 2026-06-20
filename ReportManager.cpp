#include "ReportManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>

using namespace std;

// Purpose: Constructor
ReportManager::ReportManager() {}

// Purpose: Destructor
ReportManager::~ReportManager() {}

// Purpose: Generate report of container pipeline queue
bool ReportManager::generateContainerReport(const ContainerQueue& queue) {
    try {
        filesystem::create_directories("reports");
    } catch (...) {
        return false;
    }

    ofstream file("reports/container_report.txt");
    if (!file.is_open()) return false;

    file << "=================================================================================\n"
         << "                       PORT CLEARANCE PIPELINE STATUS REPORT                     \n"
         << "=================================================================================\n\n"
         << left << setw(15) << "Container ID" 
         << setw(25) << "Cargo Description" 
         << setw(12) << "Weight (T)" 
         << setw(15) << "Category" 
         << setw(15) << "Value ($)" 
         << setw(15) << "Customs Duty ($)" 
         << setw(20) << "Importer Name" 
         << setw(15) << "Status" << "\n"
         << string(132, '-') << "\n";

    if (queue.isEmpty()) {
        file << "No containers currently in the pipeline queue.\n";
    } else {
        ContainerQueue tempQueue = queue;
        Container c;
        while (tempQueue.processNextContainer(c)) {
            file << left << setw(15) << c.id
                 << setw(25) << (c.cargoDesc.length() > 22 ? c.cargoDesc.substr(0, 19) + "..." : c.cargoDesc)
                 << setw(12) << c.weight
                 << setw(15) << c.tariffCategory
                 << setw(15) << c.value
                 << setw(15) << c.customsDuty
                 << setw(20) << c.importerName
                 << setw(15) << c.status << "\n";
        }
    }

    file << "\n=================================================================================\n"
         << "Report generated successfully.\n";
    file.close();
    cout << "[Report] Container report written to 'reports/container_report.txt'.\n";
    return true;
}

// Purpose: Generate report of registered manifests
bool ReportManager::generateManifestReport(const ManifestHash& manifestHash) {
    try {
        filesystem::create_directories("reports");
    } catch (...) {
        return false;
    }

    ofstream file("reports/manifest_report.txt");
    if (!file.is_open()) return false;

    file << "=======================================================================\n"
         << "                    SECURE CARGO MANIFEST REGISTRY REPORT              \n"
         << "=======================================================================\n\n"
         << left << setw(25) << "Manifest ID (Container ID)" 
         << setw(30) << "Stored Digital Signature (Hash Value)" << "\n"
         << string(65, '-') << "\n";

    const auto& registry = manifestHash.getRegistry();
    if (registry.empty()) {
        file << "No cargo manifests are currently registered in the database.\n";
    } else {
        for (const auto& pair : registry) {
            file << left << setw(25) << pair.first
                 << setw(30) << pair.second << "\n";
        }
    }

    file << "\n=======================================================================\n"
         << "Security integrity checks require hashes to match incoming documents.\n";
    file.close();
    cout << "[Report] Manifest report written to 'reports/manifest_report.txt'.\n";
    return true;
}

// Purpose: Generate report of yard inspectors
bool ReportManager::generateInspectorReport(const InspectorScheduler& scheduler) {
    try {
        filesystem::create_directories("reports");
    } catch (...) {
        return false;
    }

    ofstream file("reports/inspector_report.txt");
    if (!file.is_open()) return false;

    file << "========================================================\n"
         << "                 YARD INSPECTOR WORKLOAD REPORT         \n"
         << "========================================================\n\n"
         << left << setw(25) << "Inspector Name" 
         << setw(20) << "Assigned Containers" << "\n"
         << string(45, '-') << "\n";

    vector<Inspector> list = scheduler.getInspectorsList();
    if (list.empty()) {
        file << "No inspectors registered in scheduling database.\n";
    } else {
        for (const auto& ins : list) {
            file << left << setw(25) << ins.name
                 << setw(20) << ins.workload << "\n";
        }
    }

    file << "\n========================================================\n"
         << "Workload balancing is managed via a Min-Heap queue.\n";
    file.close();
    cout << "[Report] Inspector report written to 'reports/inspector_report.txt'.\n";
    return true;
}

// Purpose: Generate summary report of port statistics
bool ReportManager::generateSystemSummary(const ContainerQueue& queue,
                                      const ManifestHash& manifestHash,
                                      const InspectorScheduler& scheduler,
                                      const ShipmentSorter& sorter,
                                      const ValidationStack& stack) {
    try {
        filesystem::create_directories("reports");
    } catch (...) {
        return false;
    }

    ofstream file("reports/system_summary_report.txt");
    if (!file.is_open()) return false;

    double totalTradeValue = 0.0;
    double totalDutyCollected = 0.0;
    const auto& shipments = sorter.getShipments();
    for (const auto& s : shipments) {
        totalTradeValue += s.value;
        totalDutyCollected += s.duty;
    }

    file << "=======================================================================\n"
         << "                      CustomsOS SYSTEM SUMMARY REPORT                  \n"
         << "=======================================================================\n\n"
         << "1. DATABASE VOLUMES\n"
         << "----------------------------------------\n"
         << " * Containers in Inspection Queue : " << queue.size() << "\n"
         << " * Registered Manifest Signatures  : " << manifestHash.size() << "\n"
         << " * Active Yard Inspectors Tracked  : " << scheduler.size() << "\n"
         << " * Total Clearances in Ledger     : " << sorter.size() << "\n"
         << " * Active Audit Logs (Stack Size)  : " << stack.size() << "\n\n"
         << "2. FINANCIAL METRICS (CLEARED SHIPMENTS)\n"
         << "----------------------------------------\n"
         << " * Total Value of Trade Cleared    : $" << fixed << setprecision(2) << totalTradeValue << "\n"
         << " * Total Customs Duties Collected  : $" << totalDutyCollected << "\n\n"
         << "3. WORKLOAD ALLOCATION OVERVIEW\n"
         << "----------------------------------------\n";

    vector<Inspector> list = scheduler.getInspectorsList();
    if (list.empty()) {
        file << " (No inspectors registered)\n";
    } else {
        for (const auto& ins : list) {
            file << " * " << ins.name << " : " << ins.workload << " tasks assigned\n";
        }
    }

    file << "\n=======================================================================\n"
         << "Generated by CustomsOS Core Documentation Module.\n";
    file.close();
    cout << "[Report] System summary written to 'reports/system_summary_report.txt'.\n";
    return true;
}
