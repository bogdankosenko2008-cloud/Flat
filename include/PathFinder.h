#pragma once
#include "Apartment.h"
#include <vector>
#include <string>

struct PathResult {
    bool found = false;
    double totalDistance = 0.0;
    std::vector<Point> waypoints;
};

class PathFinder {
public:
    static PathResult findShortestPath(
        const Apartment& apartment,
        const std::string& startPremiseId,
        const std::string& targetPremiseId
    );
};