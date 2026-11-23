#include "Room.h"
#include <cmath>

double Premise::calculateArea() const {
    if (walls.empty()) return 0.0;
    double area = 0.0;
    for (const auto& w : walls) {
        area += (w.p1.x * w.p2.y - w.p2.x * w.p1.y);
    }
    return std::abs(area) / 2.0;
}

Point Premise::getCenter() const {
    if (walls.empty()) return {0.0, 0.0};
    double sumX = 0.0, sumY = 0.0;
    for (const auto& w : walls) {
        sumX += w.p1.x;
        sumY += w.p1.y;
    }
    return { sumX / walls.size(), sumY / walls.size() };
}

bool Premise::containsPoint(const Point& p) const {
    bool inside = false;
    for (const auto& w : walls) {
        if (((w.p1.y > p.y) != (w.p2.y > p.y)) &&
            (p.x < (w.p2.x - w.p1.x) * (p.y - w.p1.y) / (w.p2.y - w.p1.y) + w.p1.x)) {
            inside = !inside;
        }
    }
    return inside;
}