#include "Visualizer.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void Visualizer::printConsoleSummary(const Apartment& apartment) {
    std::cout << "\n=== APARTMENT PLAN: " << apartment.getId() << " ===\n";
    for (const auto& p : apartment.getPremises()) {
        std::cout << " - [" << p->getType() << "] ID: " << p->getId()
                  << " | Area: " << std::fixed << std::setprecision(2) << p->calculateArea() << " sq.m"
                  << " | Walls: " << p->getWalls().size()
                  << " | Doors: " << p->getDoors().size() << "\n";
    }
    std::cout << "===================================\n";
}

bool Visualizer::exportToSvg(const std::string& filePath, const Apartment& apartment, const std::vector<Point>& path) {
    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    const double scale = 60.0;
    const double offset = 40.0;

    file << "<svg xmlns=\"http://w3.org\" width=\"600\" height=\"600\" viewBox=\"0 0 600 600\">\n";
    file << "  <rect width=\"100%\" height=\"100%\" fill=\"#f8f9fa\"/>\n";

    for (const auto& p : apartment.getPremises()) {
        for (const auto& w : p->getWalls()) {
            std::string color = w.isExit ? "#e63946" : "#1d3557";
            file << "  <line x1=\"" << w.p1.x * scale + offset << "\" y1=\"" << w.p1.y * scale + offset
                 << "\" x2=\"" << w.p2.x * scale + offset << "\" y2=\"" << w.p2.y * scale + offset
                 << "\" stroke=\"" << color << "\" stroke-width=\"" << (w.width * 25.0) << "\" stroke-linecap=\"round\"/>\n";
        }
        for (const auto& d : p->getDoors()) {
            std::string dColor = d.isMainExit ? "#2a9d8f" : "#f4a261";
            file << "  <line x1=\"" << d.p1.x * scale + offset << "\" y1=\"" << d.p1.y * scale + offset
                 << "\" x2=\"" << d.p2.x * scale + offset << "\" y2=\"" << d.p2.y * scale + offset
                 << "\" stroke=\"" << dColor << "\" stroke-width=\"6\"/>\n";
        }
        Point c = p->getCenter();
        file << "  <text x=\"" << c.x * scale + offset - 20 << "\" y=\"" << c.y * scale + offset
             << "\" font-family=\"Arial\" font-size=\"12\" fill=\"#333\">" << p->getId() << "</text>\n";
    }

    if (path.size() > 1) {
        file << "  <polyline fill=\"none\" stroke=\"#e76f51\" stroke-width=\"3\" stroke-dasharray=\"5,5\" points=\"";
        for (const auto& pt : path) {
            file << pt.x * scale + offset << "," << pt.y * scale + offset << " ";
        }
        file << "\"/>\n";
    }

    file << "</svg>\n";
    return true;
}