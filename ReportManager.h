#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include "ContainerQueue.h"
#include "ManifestHash.h"
#include "InspectorScheduler.h"
#include "ShipmentSorter.h"
#include "ValidationStack.h"
#include <string>

using namespace std;

// Auditor system generating formatting report logs
class ReportManager {
public:
    ReportManager();
    ~ReportManager();

    // Export report files inside reports/ directory
    static bool generateContainerReport(const ContainerQueue& queue);
    static bool generateManifestReport(const ManifestHash& manifestHash);
    static bool generateInspectorReport(const InspectorScheduler& scheduler);
    static bool generateSystemSummary(const ContainerQueue& queue,
                                      const ManifestHash& manifestHash,
                                      const InspectorScheduler& scheduler,
                                      const ShipmentSorter& sorter,
                                      const ValidationStack& stack);
};

#endif // REPORT_MANAGER_H
