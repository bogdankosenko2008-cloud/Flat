#include "Apartment.h"
#include <sstream>

std::vector<std::string> Apartment::optimizeAndValidate(const OptimizationRequirements& req) {
    std::vector<std::string> report;

    for (auto& p : premises) {
        double area = p->calculateArea();
        if (p->getType() == "room" && area < req.minRoomArea) {
            std::ostringstream oss;
            oss << "[Warning] Room '" << p->getId() << "' area is " << area 
                << " sq.m (min required: " << req.minRoomArea << "). Needs expansion.";
            report.push_back(oss.str());
        }

        for (const auto& d : p->getDoors()) {
            if (d.getWidth() < req.minDoorWidth) {
                std::ostringstream oss;
                oss << "[Optimization] Door in '" << p->getId() 
                    << "' is too narrow (" << d.getWidth() << " m). Recommended: " 
                    << req.minDoorWidth << " m.";
                report.push_back(oss.str());
            }
        }
    }

    if (report.empty()) {
        report.push_back("Layout meets all optimization requirements.");
    }

    return report;
}