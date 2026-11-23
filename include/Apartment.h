#pragma once
#include "Room.h"
#include <vector>
#include <memory>
#include <string>

struct OptimizationRequirements {
    double minRoomArea = 9.0;
    double minCorridorWidth = 1.2;
    double minDoorWidth = 0.8;
    bool requireDirectCorridorAccess = true;
};

class Apartment {
private:
    std::string apartmentId;
    std::vector<std::shared_ptr<Premise>> premises;

public:
    Apartment(const std::string& id = "") : apartmentId(id) {}

    void setId(const std::string& id) { apartmentId = id; }
    const std::string& getId() const { return apartmentId; }

    void addPremise(std::shared_ptr<Premise> premise) {
        premises.push_back(premise);
    }

    const std::vector<std::shared_ptr<Premise>>& getPremises() const {
        return premises;
    }

    std::vector<std::string> optimizeAndValidate(const OptimizationRequirements& req);
};