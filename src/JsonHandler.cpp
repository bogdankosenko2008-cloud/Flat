#include "JsonHandler.h"
#include <sstream>
#include <iomanip>

bool JsonHandler::loadApartment(const std::string& filePath, Apartment& apartment) {
    std::string content = SimpleJson::readFile(filePath);
    if (content.empty()) return false;

    apartment.setId(SimpleJson::getString(content, "apartment_id"));
    auto premiseBlocks = SimpleJson::getObjectArray(content, "premises");

    for (const auto& block : premiseBlocks) {
        std::string id = SimpleJson::getString(block, "id");
        std::string type = SimpleJson::getString(block, "type");
        std::string name = SimpleJson::getString(block, "name");

        std::shared_ptr<Premise> premise;
        if (type == "corridor") {
            premise = std::make_shared<Corridor>(id, name);
        } else {
            premise = std::make_shared<Room>(id, name);
        }

        auto wallBlocks = SimpleJson::getObjectArray(block, "walls");
        for (const auto& wb : wallBlocks) {
            Wall w;
            w.p1.x = SimpleJson::getNumber(wb, "x1");
            w.p1.y = SimpleJson::getNumber(wb, "y1");
            w.p2.x = SimpleJson::getNumber(wb, "x2");
            w.p2.y = SimpleJson::getNumber(wb, "y2");
            w.isExit = SimpleJson::getBool(wb, "isExit");
            w.doorsCount = static_cast<int>(SimpleJson::getNumber(wb, "doors_count"));
            w.width = SimpleJson::getNumber(wb, "width");
            premise->addWall(w);
        }

        auto doorBlocks = SimpleJson::getObjectArray(block, "doors");
        for (const auto& db : doorBlocks) {
            Door d;
            d.p1.x = SimpleJson::getNumber(db, "x1");
            d.p1.y = SimpleJson::getNumber(db, "y1");
            d.p2.x = SimpleJson::getNumber(db, "x2");
            d.p2.y = SimpleJson::getNumber(db, "y2");
            d.material = SimpleJson::getString(db, "material");
            d.isMainExit = SimpleJson::getBool(db, "isMainExit");
            premise->addDoor(d);
        }

        apartment.addPremise(premise);
    }
    return true;
}

bool JsonHandler::loadRequirements(const std::string& filePath, OptimizationRequirements& optReq, SimulationConfig& simConfig) {
    std::string content = SimpleJson::readFile(filePath);
    if (content.empty()) return false;

    optReq.minRoomArea = SimpleJson::getNumber(content, "min_room_area");
    optReq.minCorridorWidth = SimpleJson::getNumber(content, "min_corridor_width");
    optReq.minDoorWidth = SimpleJson::getNumber(content, "min_door_width");
    optReq.requireDirectCorridorAccess = SimpleJson::getBool(content, "require_direct_corridor_access");

    simConfig.startPremiseId = SimpleJson::getString(content, "start_premise_id");
    simConfig.targetPremiseId = SimpleJson::getString(content, "target_premise_id");
    simConfig.trialsPerStepCount = static_cast<int>(SimpleJson::getNumber(content, "trials_per_step_count"));
    simConfig.stepLength = SimpleJson::getNumber(content, "step_length");
    simConfig.memorySize = static_cast<int>(SimpleJson::getNumber(content, "memory_size"));

    auto steps = SimpleJson::getIntArray(content, "step_counts_to_test");
    if (!steps.empty()) {
        simConfig.stepCountsToTest = steps;
    }
    return true;
}

bool JsonHandler::saveOutput(
    const std::string& filePath,
    const Apartment& apartment,
    const std::vector<std::string>& optimizationReport,
    const std::vector<Point>& optimalPath,
    double pathDistance,
    const std::vector<SimulationResultRow>& simResults
) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "{\n";
    oss << "  \"apartment_id\": \"" << apartment.getId() << "\",\n";
    oss << "  \"premises_count\": " << apartment.getPremises().size() << ",\n";

    oss << "  \"optimization_report\": [\n";
    for (size_t i = 0; i < optimizationReport.size(); ++i) {
        oss << "    \"" << optimizationReport[i] << "\"" << (i + 1 < optimizationReport.size() ? "," : "") << "\n";
    }
    oss << "  ],\n";

    oss << "  \"optimal_path\": {\n";
    oss << "    \"total_distance\": " << pathDistance << ",\n";
    oss << "    \"waypoints\": [\n";
    for (size_t i = 0; i < optimalPath.size(); ++i) {
        oss << "      { \"x\": " << optimalPath[i].x << ", \"y\": " << optimalPath[i].y << " }"
            << (i + 1 < optimalPath.size() ? "," : "") << "\n";
    }
    oss << "    ]\n";
    oss << "  },\n";

    oss << "  \"robot_simulation_results\": [\n";
    for (size_t i = 0; i < simResults.size(); ++i) {
        oss << "    { \"max_steps\": " << simResults[i].steps << ", \"success_rate_percent\": " << simResults[i].successRate << " }"
            << (i + 1 < simResults.size() ? "," : "") << "\n";
    }
    oss << "  ]\n";
    oss << "}\n";

    return SimpleJson::writeFile(filePath, oss.str());
}