#include <iostream>
#include <iomanip>
#include "Apartment.h"
#include "JsonHandler.h"
#include "Visualizer.h"
#include "PathFinder.h"
#include "Robot.h"

int main() {
    Apartment apartment;
    OptimizationRequirements optReq;
    SimulationConfig simConfig;

    std::cout << "=== 1. LOADING INPUT DATA ===" << std::endl;
    if (!JsonHandler::loadApartment("data/apartment_input.json", apartment)) {
        std::cerr << "Error: Failed to load data/apartment_input.json" << std::endl;
        return 1;
    }
    if (!JsonHandler::loadRequirements("data/requirements.json", optReq, simConfig)) {
        std::cerr << "Error: Failed to load data/requirements.json" << std::endl;
        return 1;
    }

    Visualizer::printConsoleSummary(apartment);

    std::cout << "\n=== 2. LAYOUT OPTIMIZATION & VALIDATION ===" << std::endl;
    auto optimizationReport = apartment.optimizeAndValidate(optReq);
    for (const auto& msg : optimizationReport) {
        std::cout << " * " << msg << std::endl;
    }

    std::cout << "\n=== 3. OPTIMAL PATHFINDING (DIJKSTRA) ===" << std::endl;
    PathResult pathRes = PathFinder::findShortestPath(
        apartment,
        simConfig.startPremiseId,
        simConfig.targetPremiseId
    );

    if (pathRes.found) {
        std::cout << "Shortest path from '" << simConfig.startPremiseId
                  << "' to '" << simConfig.targetPremiseId << "' found!" << std::endl;
        std::cout << "Total distance: " << std::fixed << std::setprecision(2)
                  << pathRes.totalDistance << " m" << std::endl;
        std::cout << "Waypoints: ";
        for (size_t i = 0; i < pathRes.waypoints.size(); ++i) {
            std::cout << "(" << pathRes.waypoints[i].x << ", " << pathRes.waypoints[i].y << ")"
                      << (i + 1 < pathRes.waypoints.size() ? " -> " : "");
        }
        std::cout << std::endl;
    } else {
        std::cout << "Warning: No valid path found between specified premises." << std::endl;
    }

    std::cout << "\n=== 4. ROBOT NAVIGATION SIMULATION ===" << std::endl;
    auto simResults = RobotSimulator::runExperiments(apartment, simConfig);
    for (const auto& row : simResults) {
        std::cout << " - Max steps: " << std::setw(4) << row.steps
                  << " | Exit reached rate: " << std::fixed << std::setprecision(1)
                  << row.successRate << "%" << std::endl;
    }

    std::cout << "\n=== 5. SAVING RESULTS & VISUALIZATION ===" << std::endl;
    if (Visualizer::exportToSvg("data/apartment_plan.svg", apartment, pathRes.waypoints)) {
        std::cout << "[OK] Floor plan with optimal route saved to: data/apartment_plan.svg" << std::endl;
    }
    if (JsonHandler::saveOutput(
            "data/simulation_output.json",
            apartment,
            optimizationReport,
            pathRes.waypoints,
            pathRes.totalDistance,
            simResults)) {
        std::cout << "[OK] Full report saved to: data/simulation_output.json" << std::endl;
    }

    return 0;
}