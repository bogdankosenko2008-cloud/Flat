#pragma once
#include "Apartment.h"
#include <vector>
#include <string>

class Visualizer {
public:
    static void printConsoleSummary(const Apartment& apartment);
    static bool exportToSvg(const std::string& filePath, const Apartment& apartment, const std::vector<Point>& path = {});
};