#include "Robot.h"
#include <random>
#include <cmath>
#include <limits>
#include <algorithm>

namespace {
    const double PI = 3.14159265358979323846;

    // Перевірка, чи знаходиться точка всередині будь-якого приміщення або біля дверей
    bool isValidPosition(const Apartment& apartment, const Point& p) {
        for (const auto& premise : apartment.getPremises()) {
            if (premise->containsPoint(p)) return true;
            for (const auto& d : premise->getDoors()) {
                if (p.distanceTo(d.getCenter()) <= d.getWidth() / 2.0 + 0.15) {
                    return true;
                }
            }
        }
        return false;
    }

    // Перевірка, чи досяг робот головного виходу з квартири
    bool reachedMainExit(const Apartment& apartment, const Point& p) {
        for (const auto& premise : apartment.getPremises()) {
            for (const auto& d : premise->getDoors()) {
                if (d.isMainExit && p.distanceTo(d.getCenter()) <= 0.6) {
                    return true;
                }
            }
        }
        return false;
    }
}

Robot::Robot(Point startPos, double stepLen, size_t memSize)
    : currentPos(startPos), stepLength(stepLen), maxMemorySize(memSize) {
    memory.push_back(startPos);
}

bool Robot::makeStep(const Apartment& apartment) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> angleDist(0.0, 2.0 * PI);

    const int candidateCount = 16;
    Point bestCandidate = currentPos;
    double bestMinDist = -1.0;
    bool foundValid = false;

    for (int i = 0; i < candidateCount; ++i) {
        double angle = angleDist(rng);
        Point candidate = {
            currentPos.x + stepLength * std::cos(angle),
            currentPos.y + stepLength * std::sin(angle)
        };

        if (!isValidPosition(apartment, candidate)) continue;

        // Обчислюємо мінімальну відстань від кандидата до точок у пам'яті
        double minDistToHistory = std::numeric_limits<double>::infinity();
        for (const auto& pastPt : memory) {
            minDistToHistory = std::min(minDistToHistory, candidate.distanceTo(pastPt));
        }

        if (minDistToHistory > bestMinDist) {
            bestMinDist = minDistToHistory;
            bestCandidate = candidate;
            foundValid = true;
        }
    }

    if (!foundValid) return false;

    currentPos = bestCandidate;
    memory.push_back(currentPos);
    if (memory.size() > maxMemorySize) {
        memory.pop_front();
    }
    return true;
}

std::vector<SimulationResultRow> RobotSimulator::runExperiments(
    const Apartment& apartment,
    const SimulationConfig& config
) {
    std::vector<SimulationResultRow> results;

    // Знаходимо стартову точку (центр стартового приміщення)
    Point startPos = {1.0, 1.0};
    for (const auto& p : apartment.getPremises()) {
        if (p->getId() == config.startPremiseId) {
            startPos = p->getCenter();
            break;
        }
    }

    for (int maxSteps : config.stepCountsToTest) {
        int successCount = 0;

        for (int trial = 0; trial < config.trialsPerStepCount; ++trial) {
            Robot robot(startPos, config.stepLength, config.memorySize);
            for (int step = 0; step < maxSteps; ++step) {
                robot.makeStep(apartment);
                if (reachedMainExit(apartment, robot.getPosition())) {
                    successCount++;
                    break;
                }
            }
        }

        double rate = (config.trialsPerStepCount > 0)
            ? (100.0 * successCount / config.trialsPerStepCount)
            : 0.0;
        results.push_back({maxSteps, rate});
    }

    return results;
}