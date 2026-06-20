#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "TariffTree.h"
#include "ValidationStack.h"
#include "ContainerQueue.h"
#include "ManifestHash.h"
#include "ShipmentSorter.h"
#include "InspectionGraph.h"
#include "InspectorScheduler.h"
#include <string>

using namespace std;

// Central file manager coordinating CSV saves/loads
class DataManager {
public:
    DataManager();
    ~DataManager();

    // Read all databases from file to memory
    static bool loadAllData(TariffTree& tree,
                            ValidationStack& stack,
                            ContainerQueue& queue,
                            ManifestHash& manifestHash,
                            ShipmentSorter& sorter,
                            InspectionGraph& graph,
                            InspectorScheduler& scheduler);

    // Save all databases from memory to file
    static bool saveAllData(const TariffTree& tree,
                            const ValidationStack& stack,
                            const ContainerQueue& queue,
                            const ManifestHash& manifestHash,
                            const ShipmentSorter& sorter,
                            const InspectionGraph& graph,
                            const InspectorScheduler& scheduler);

    // Seed empty CSV files with default dataset
    static void initializeDefaultData();
};

#endif // DATA_MANAGER_H
