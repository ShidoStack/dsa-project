#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>

#include "TariffTree.h"
#include "ValidationStack.h"
#include "ContainerQueue.h"
#include "ManifestHash.h"
#include "ShipmentSorter.h"
#include "InspectionGraph.h"
#include "RouteFinder.h"
#include "InspectorScheduler.h"
#include "DataManager.h"
#include "ReportManager.h"

using namespace std;

// Safe input helpers
string readLine() {
    string s;
    getline(cin, s);
    return s;
}

int readInt() {
    int val;
    while (true) {
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid entry. Please enter a valid integer: ";
        }
    }
}

double readDouble() {
    double val;
    while (true) {
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid entry. Please enter a valid number: ";
        }
    }
}

void printMainMenu() {
    cout << "\n========================================================\n"
         << "          CustomsOS - Trade Documentation Tracker      \n"
         << "========================================================\n"
         << " 1.  Tariff Classification Tree\n"
         << " 2.  Validation Records\n"
         << " 3.  Container Queue\n"
         << " 4.  Manifest Verification\n"
         << " 5.  Shipment Sorting\n"
         << " 6.  Inspection Network\n"
         << " 7.  Fastest Route Finder\n"
         << " 8.  Inspector Scheduler\n"
         << " 9.  Complete Clearance Simulation\n"
         << " 10. Generate Audit Reports\n"
         << " 11. Save & Exit\n"
         << "========================================================\n"
         << "Enter choice (1-11): ";
}

// Guided validation step selection menu
string selectValidationStep() {
    cout << "\nSelect Validation Step:\n"
         << "  1. Document Verification\n"
         << "  2. Customs Office\n"
         << "  3. Security Screening\n"
         << "  4. Weight Verification\n"
         << "  5. Quality Inspection\n"
         << "  6. Cargo Sealing\n"
         << "  7. Hazard Check\n"
         << "  8. Export Clearance\n"
         << "  9. Import Clearance\n"
         << "  10. Manual Entry\n"
         << "Choose option: ";

    int opt = readInt();

    string steps[] = {
        "Document Verification",
        "Customs Office",
        "Security Screening",
        "Weight Verification",
        "Quality Inspection",
        "Cargo Sealing",
        "Hazard Check",
        "Export Clearance",
        "Import Clearance"
    };

    if (opt >= 1 && opt <= 9) {
        string selected = steps[opt - 1];
        cout << "Selected Step: " << selected << "\n";
        return selected;
    } else {
        cout << "Enter custom step description: ";
        return readLine();
    }
}

int main() {
    TariffTree tree;
    ValidationStack stack;
    ContainerQueue queue;
    ManifestHash manifestHash;
    ShipmentSorter sorter;
    InspectionGraph graph;
    InspectorScheduler scheduler;

    cout << "Initializing CustomsOS...\n";
    if (DataManager::loadAllData(tree, stack, queue, manifestHash, sorter, graph, scheduler)) {
        cout << "All databases loaded successfully.\n";
    } else {
        cout << "Notice: Seeding new default data files.\n";
    }

    int choice = 0;
    while (true) {
        printMainMenu();
        choice = readInt();

        switch (choice) {

            case 1: {
                int treeChoice = 0;
                do {
                    cout << "\n--- Tariff Classification Tree Sub-Menu ---\n"
                         << "1. Display Tree\n"
                         << "2. Add New Commodity Category\n"
                         << "3. Search for Category Rate\n"
                         << "4. Return to Main Menu\n"
                         << "Enter choice (1-4): ";
                    treeChoice = readInt();

                    if (treeChoice == 1) {
                        tree.displayTree();

                    } else if (treeChoice == 2) {
                        cout << "Enter parent category (e.g. Electronics, Food, Root): ";
                        string parent = readLine();
                        cout << "Enter new subcategory name: ";
                        string child = readLine();
                        cout << "Enter customs duty rate percentage (e.g. 6.5): ";
                        double rate = readDouble();

                        if (tree.addCategory(parent, child, rate)) {
                            cout << "Category '" << child << "' added under '" << parent << "'.\n";
                        } else {
                            cout << "Error: Parent not found or category already exists.\n";
                        }

                    } else if (treeChoice == 3) {
                        cout << "Enter category name to search: ";
                        string catName = readLine();
                        TariffNode* node = tree.searchCategory(catName);
                        if (node) {
                            cout << "Category Found : [" << node->name << "]\n"
                                 << "Duty Rate      : " << node->dutyRate << "%\n";
                        } else {
                            cout << "Category not found in the tariff registry.\n";
                        }
                    }
                } while (treeChoice != 4);
                break;
            }

            case 2: {
                int stackChoice = 0;
                do {
                    cout << "\n--- Validation Records Sub-Menu ---\n"
                         << "1. View All Validation Steps (Latest First)\n"
                         << "2. Add Validation Step (Push)\n"
                         << "3. Undo Last Validation Step (Pop)\n"
                         << "4. View Most Recent Step (Top)\n"
                         << "5. Return to Main Menu\n"
                         << "Enter choice (1-5): ";
                    stackChoice = readInt();

                    if (stackChoice == 1) {
                        stack.displayAllSteps();

                    } else if (stackChoice == 2) {
                        cout << "Enter Container ID: ";
                        string cid = readLine();

                        // Guided step selection menu
                        string desc = selectValidationStep();

                        cout << "Enter Status (Passed/Failed): ";
                        string stat = readLine();
                        cout << "Enter Timestamp (YYYY-MM-DD HH:MM): ";
                        string tstamp = readLine();

                        stack.addValidationStep(cid, desc, stat, tstamp);
                        cout << "Validation step logged.\n";

                    } else if (stackChoice == 3) {
                        if (stack.undoLastValidation()) {
                            cout << "Rollback Successful! Last validation step removed.\n";
                        } else {
                            cout << "Undo Failed: History is empty.\n";
                        }

                    } else if (stackChoice == 4) {
                        ValidationStep step("", "", "", "");
                        if (stack.viewLatestStep(step)) {
                            cout << "Latest Step:\n"
                                 << " - Container ID : " << step.containerId << "\n"
                                 << " - Description  : " << step.stepDescription << "\n"
                                 << " - Status       : " << step.status << "\n"
                                 << " - Timestamp    : " << step.timestamp << "\n";
                        } else {
                            cout << "No validation steps in history.\n";
                        }
                    }
                } while (stackChoice != 5);
                break;
            }

            case 3: {
                int qChoice = 0;
                do {
                    cout << "\n--- Checking Pipeline Sub-Menu ---\n"
                         << "1. Display Clearance Queue\n"
                         << "2. Enqueue New Container (Arrival)\n"
                         << "3. Dequeue/Process Next Container\n"
                         << "4. Return to Main Menu\n"
                         << "Enter choice (1-4): ";
                    qChoice = readInt();

                    if (qChoice == 1) {
                        queue.displayQueue();

                    } else if (qChoice == 2) {
                        cout << "Enter Container ID (e.g. CONT-200): ";
                        string cid = readLine();
                        cout << "Enter Cargo Description: ";
                        string desc = readLine();
                        cout << "Enter Weight in Tons: ";
                        double weight = readDouble();
                        cout << "Enter Tariff Category (e.g. iPhone, Dairy, Machinery): ";
                        string cat = readLine();
                        cout << "Enter Declared Cargo Value ($): ";
                        double val = readDouble();
                        cout << "Enter Importer Name: ";
                        string imp = readLine();

                        double dutyRate = tree.getDutyRate(cat);
                        double calculatedDuty = val * (dutyRate / 100.0);

                        Container c(cid, desc, weight, cat, val, calculatedDuty, imp, "Pending");
                        queue.enqueueContainer(c);
                        cout << "Container enqueued. Calculated duty: $" << calculatedDuty
                             << " (" << dutyRate << "%).\n";

                    } else if (qChoice == 3) {
                        Container c;
                        if (queue.processNextContainer(c)) {
                            cout << "Processing Container " << c.id
                                 << " (" << c.cargoDesc << ")...\n"
                                 << "Status: " << c.status << "\n";
                        } else {
                            cout << "Queue is empty. No containers to process.\n";
                        }
                    }
                } while (qChoice != 4);
                break;
            }

            case 4: {
                int hashChoice = 0;
                do {
                    cout << "\n--- Manifest Verification Sub-Menu ---\n"
                         << "1. Display Stored Manifest Signatures\n"
                         << "2. Register New Manifest Document\n"
                         << "3. Verify Manifest Document Content\n"
                         << "4. Return to Main Menu\n"
                         << "Enter choice (1-4): ";
                    hashChoice = readInt();

                    if (hashChoice == 1) {
                        manifestHash.displayManifests();

                    } else if (hashChoice == 2) {
                        cout << "Enter Manifest ID (Container ID): ";
                        string mid = readLine();
                        cout << "Enter Manifest Declaration Details (full text to hash):\n";
                        string details = readLine();

                        manifestHash.registerManifest(mid, details);
                        cout << "Manifest registered. Hash signature: "
                             << manifestHash.calculateHash(details) << "\n";

                    } else if (hashChoice == 3) {
                        cout << "Enter Manifest ID to verify: ";
                        string mid = readLine();
                        cout << "Enter manifest content text: ";
                        string details = readLine();

                        if (manifestHash.verifyManifest(mid, details)) {
                            cout << "[Verification SUCCESS] Document is authentic.\n";
                        } else {
                            cout << "[Verification FAIL] Document failed validation check.\n";
                        }
                    }
                } while (hashChoice != 4);
                break;
            }

            case 5: {
                int sortChoice = 0;
                do {
                    cout << "\n--- Shipment Sorter Sub-Menu ---\n"
                         << "1. Display Shipments Ledger\n"
                         << "2. Sort by Customs Duty (Ascending)\n"
                         << "3. Sort by Customs Duty (Descending)\n"
                         << "4. Add Shipment Record\n"
                         << "5. Return to Main Menu\n"
                         << "Enter choice (1-5): ";
                    sortChoice = readInt();

                    if (sortChoice == 1) {
                        sorter.displayShipments();

                    } else if (sortChoice == 2) {
                        sorter.sortAscending();
                        cout << "Sorted in ascending duty order.\n";
                        sorter.displayShipments();

                    } else if (sortChoice == 3) {
                        sorter.sortDescending();
                        cout << "Sorted in descending duty order.\n";
                        sorter.displayShipments();

                    } else if (sortChoice == 4) {
                        cout << "Enter Shipment ID: ";
                        string sid = readLine();
                        cout << "Enter Description: ";
                        string desc = readLine();
                        cout << "Enter Category: ";
                        string cat = readLine();
                        cout << "Enter Cargo Value ($): ";
                        double val = readDouble();
                        cout << "Enter Customs Duty ($): ";
                        double duty = readDouble();
                        cout << "Enter Importer: ";
                        string imp = readLine();

                        sorter.addShipment(Shipment(sid, desc, cat, val, duty, imp));
                        cout << "Shipment record added.\n";
                    }
                } while (sortChoice != 5);
                break;
            }

            case 6: {
                int gChoice = 0;
                do {
                    cout << "\n--- Inspection Network Sub-Menu ---\n"
                         << "1. Display Graph (Adjacency List)\n"
                         << "2. Add Route (Edge)\n"
                         << "3. Add Checkpoint (Vertex)\n"
                         << "4. Return to Main Menu\n"
                         << "Enter choice (1-4): ";
                    gChoice = readInt();

                    if (gChoice == 1) {
                        graph.displayNetwork();

                    } else if (gChoice == 2) {
                        cout << "Enter Origin Checkpoint: ";
                        string src = readLine();
                        cout << "Enter Destination Checkpoint: ";
                        string dest = readLine();
                        cout << "Enter wait/transit time in hours: ";
                        double wait = readDouble();

                        graph.addRoute(src, dest, wait);
                        cout << "Route connected: [" << src << "] ──(" << wait << " hrs)──> [" << dest << "].\n";

                    } else if (gChoice == 3) {
                        cout << "Enter new checkpoint name: ";
                        string cp = readLine();
                        graph.addInspectionPoint(cp);
                        cout << "Checkpoint registered.\n";
                    }
                } while (gChoice != 4);
                break;
            }

            case 7: {
                cout << "\n--- Fastest Route Finder ---\n";
                cout << "Enter Start Checkpoint (e.g. Mumbai_Port): ";
                string src = readLine();
                cout << "Enter End Checkpoint (e.g. Hamburg_Port): ";
                string dest = readLine();

                vector<string> path;
                double totalWait = 0.0;
                if (RouteFinder::findShortestRoute(graph, src, dest, path, totalWait)) {
                    RouteFinder::displayRoute(path, totalWait);
                } else {
                    cout << "Error: No route exists between [" << src << "] and [" << dest << "].\n";
                }
                break;
            }

            case 8: {
                int schedChoice = 0;
                do {
                    cout << "\n--- Inspector Scheduler Sub-Menu ---\n"
                         << "1. View Inspector Workloads (Min-Heap)\n"
                         << "2. Register New Inspector\n"
                         << "3. Return to Main Menu\n"
                         << "Enter choice (1-3): ";
                    schedChoice = readInt();

                    if (schedChoice == 1) {
                        scheduler.displayInspectorWorkloads();

                    } else if (schedChoice == 2) {
                        cout << "Enter Inspector Name: ";
                        string name = readLine();
                        cout << "Enter Initial Workload (assigned tasks): ";
                        int work = readInt();

                        scheduler.addInspector(name, work);
                        cout << "Inspector " << name << " added to Min-Heap scheduler.\n";
                    }
                } while (schedChoice != 3);
                break;
            }

            case 9: {
                cout << "\n========================================================\n"
                     << "            COMPLETE PORT CLEARANCE SIMULATION          \n"
                     << "========================================================\n";

                if (queue.isEmpty()) {
                    cout << "No containers in the Checking Pipeline queue.\n"
                         << "Please enqueue a container first under option 3.\n";
                    break;
                }

                // Step 1: Fetch front container
                Container container;
                queue.getFront(container);
                cout << "[Step 1/8] Container Arrived: " << container.id << "\n"
                     << "           Cargo Description : " << container.cargoDesc << "\n"
                     << "           Importer          : " << container.importerName << "\n"
                     << "           Declared Value    : $" << container.value << "\n"
                     << "           Weight            : " << container.weight << " Tons\n\n";

                // Step 2: Manifest Verification
                cout << "[Step 2/8] Manifest Verification Security Check...\n"
                     << "Options:\n"
                     << "  1. Autocomplete with correct manifest text\n"
                     << "  2. Type custom manifest content manually\n"
                     << "Select option (1-2): ";
                int verifyOpt = readInt();
                string docContent = "";

                if (verifyOpt == 1) {
                    if (container.id == "CONT-101") {
                        docContent = "Apple Inc. Cargo Manifest: 500x iPhones, weight 2.4 tons, value $150000";
                    } else if (container.id == "CONT-102") {
                        docContent = "Fruit Importers LLC Cargo Manifest: 5000x Fresh Apples, weight 12.0 tons, value $20000";
                    } else if (container.id == "CONT-103") {
                        docContent = "Industrial Imports Cargo Manifest: 1x Industrial CNC Mill, weight 2.5 tons, value $100000";
                    } else if (container.id == "CONT-104") {
                        docContent = "ToolPro Corp Cargo Manifest: 150x Precision Toolsets, weight 1.2 tons, value $30000";
                    } else {
                        docContent = "Generic manifest content for container " + container.id;
                        manifestHash.registerManifest(container.id, docContent);
                    }
                } else {
                    cout << "Enter manifest content: ";
                    docContent = readLine();
                }

                cout << "Checking digital signature against registry hash...\n";
                if (!manifestHash.verifyManifest(container.id, docContent)) {
                    cout << "\n[SIMULATION ABORTED] Security Integrity Violation! Container blocked.\n";
                    stack.addValidationStep(container.id, "Security Manifest Check", "Failed", "2026-06-18_09:00");
                    break;
                }
                cout << "Manifest Signature MATCHED. Container cleared for classification.\n\n";

                // Step 3: Tariff Classification
                cout << "[Step 3/8] Classifying commodity tariff code...\n";
                TariffNode* node = tree.searchCategory(container.tariffCategory);
                if (!node) {
                    cout << "\n[SIMULATION ABORTED] Tariff classification error! Category '"
                         << container.tariffCategory << "' not in tree.\n";
                    stack.addValidationStep(container.id, "Tariff Classification", "Failed", "2026-06-18_09:00");
                    break;
                }
                cout << "Tariff Code Identified : [" << node->name << "]\n"
                     << "Matched Rate           : " << node->dutyRate << "%\n\n";

                // Step 4: Duty Calculation
                cout << "[Step 4/8] Customs Duty Calculation...\n";
                double calculatedDuty = container.value * (node->dutyRate / 100.0);
                container.customsDuty = calculatedDuty;
                cout << "Formula   : Cargo Value ($" << container.value
                     << ") x Rate (" << node->dutyRate << "%)\n"
                     << "Duty Owed : $" << calculatedDuty << "\n\n";

                // Step 5: Inspector Assignment
                cout << "[Step 5/8] Scheduling Yard Inspector...\n";
                string assignedInspector = "";
                if (!scheduler.assignContainer(container.id, assignedInspector)) {
                    cout << "[SIMULATION ABORTED] Workload scheduler error.\n";
                    break;
                }
                cout << "Inspector '" << assignedInspector << "' will coordinate inspection.\n\n";

                // Step 6: Route Optimization
                cout << "[Step 6/8] Optimizing transit path through checkpoints...\n";
                vector<string> path;
                double waitTime = 0.0;
                if (!RouteFinder::findShortestRoute(graph, "Mumbai_Port", "Hamburg_Port", path, waitTime)) {
                    cout << "[SIMULATION ABORTED] No route from Mumbai_Port to Hamburg_Port.\n";
                    break;
                }
                RouteFinder::displayRoute(path, waitTime);
                cout << "\n";

                // Step 7: Audit Log
                cout << "[Step 7/8] Logging validation trail in Audit Stack...\n";
                stack.addValidationStep(container.id, "Integrated Simulation Clear", "Passed", "2026-06-18_09:05");
                cout << "Validation pushed onto stack.\n\n";

                // Step 8: Complete Clearance
                cout << "[Step 8/8] Completing clearance...\n";
                Container popped;
                queue.processNextContainer(popped);
                sorter.addShipment(Shipment(
                    container.id, container.cargoDesc, container.tariffCategory,
                    container.value, container.customsDuty, container.importerName
                ));

                cout << "========================================================\n"
                     << "      CLEARANCE SUCCESSFUL! Container has exited port.  \n"
                     << "========================================================\n";
                break;
            }

            case 10: {
                cout << "\nExporting audit reports...\n";
                ReportManager::generateContainerReport(queue);
                ReportManager::generateManifestReport(manifestHash);
                ReportManager::generateInspectorReport(scheduler);
                ReportManager::generateSystemSummary(queue, manifestHash, scheduler, sorter, stack);
                cout << "All reports written to 'reports/'.\n";
                break;
            }

            case 11: {
                cout << "\nSaving system databases to CSV...\n";
                if (DataManager::saveAllData(tree, stack, queue, manifestHash, sorter, graph, scheduler)) {
                    cout << "Data saved successfully. Exiting CustomsOS.\n";
                } else {
                    cout << "Warning: Some files failed to save properly.\n";
                }
                return 0;
            }

            default:
                cout << "Invalid choice. Please select between 1 and 11.\n";
        }
    }

    return 0;
}
