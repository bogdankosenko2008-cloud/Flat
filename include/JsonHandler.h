#pragma once
#include "Apartment.h"
#include "SimpleJson.h"
#include <string>
#include <vector>

struct SimulationConfig {
    std::string startPremiseId;
    std::string targetPremiseId;
    int trialsPerStepCount = 100;
    std::vector<int> stepCountsToTest = {10, 50, 100, 200};
    double stepLength = 0.5;
    int memorySize = 10;
};

struct SimulationResultRow {
    int steps;
    double successRate;
};

class JsonHandler {
public:
    static bool loadApartment(const std::string& filePath, Apartment& apartment);
    static bool loadRequirements(const std::string& filePath, OptimizationRequirements& optReq, SimulationConfig& simConfig);
    static bool saveOutput(
        const std::string& filePath,
        const Apartment& apartment,
        const std::vector<std::string>& optimizationReport,
        const std::vector<Point>& optimalPath,
        double pathDistance,
        const std::vector<SimulationResultRow>& simResults
    );
};