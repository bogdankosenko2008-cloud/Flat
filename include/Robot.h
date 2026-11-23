#pragma once
#include "Apartment.h"
#include "JsonHandler.h"
#include <deque>
#include <vector>

class Robot {
private:
    Point currentPos;
    double stepLength;
    size_t maxMemorySize;
    std::deque<Point> memory;

public:
    Robot(Point startPos, double stepLen, size_t memSize);

    // Виконати один розумний крок (максимізація відстані до недавніх точок)
    bool makeStep(const Apartment& apartment);

    Point getPosition() const { return currentPos; }
};

class RobotSimulator {
public:
    static std::vector<SimulationResultRow> runExperiments(
        const Apartment& apartment,
        const SimulationConfig& config
    );
};