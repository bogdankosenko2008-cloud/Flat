#pragma once
#include <string>
#include <cmath>

struct Point {
    double x = 0.0;
    double y = 0.0;

    double distanceTo(const Point& other) const {
        return std::hypot(x - other.x, y - other.y);
    }
};

struct Door {
    Point p1;
    Point p2;
    std::string material;
    bool isMainExit = false;

    double getWidth() const {
        return p1.distanceTo(p2);
    }

    Point getCenter() const {
        return { (p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0 };
    }
};

struct Wall {
    Point p1;
    Point p2;
    bool isExit = false;
    int doorsCount = 0;
    double width = 0.15;

    double getLength() const {
        return p1.distanceTo(p2);
    }
};