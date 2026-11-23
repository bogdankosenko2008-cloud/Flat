#include <iostream>
#include <cassert>
#include <cmath>
#include "Geometry.h"
#include "Room.h"
#include "Apartment.h"
#include "PathFinder.h"
#include "Robot.h"

void testGeometryAndRoomArea() {
    Room r("r1", "TestRoom");
    r.addWall({{0, 0}, {4, 0}, false, 0, 0.2});
    r.addWall({{4, 0}, {4, 3}, false, 0, 0.2});
    r.addWall({{4, 3}, {0, 3}, false, 0, 0.2});
    r.addWall({{0, 3}, {0, 0}, false, 0, 0.2});

    assert(std::abs(r.calculateArea() - 12.0) < 1e-6);
    assert(r.containsPoint({2.0, 1.5}) == true);
    assert(r.containsPoint({5.0, 5.0}) == false);
    std::cout << "[PASS] testGeometryAndRoomArea" << std::endl;
}

void testPathFinder() {
    Apartment apt("TestApt");

    auto c1 = std::make_shared<Corridor>("c1", "Corridor");
    c1->addWall({{0, 0}, {2, 0}});
    c1->addWall({{2, 0}, {2, 4}});
    c1->addWall({{2, 4}, {0, 4}});
    c1->addWall({{0, 4}, {0, 0}});
    c1->addDoor({{2, 1.5}, {2, 2.5}, "wood", false});

    auto r1 = std::make_shared<Room>("r1", "Bedroom");
    r1->addWall({{2, 0}, {6, 0}});
    r1->addWall({{6, 0}, {6, 4}});
    r1->addWall({{6, 4}, {2, 4}});
    r1->addWall({{2, 4}, {2, 0}});
    r1->addDoor({{2, 1.5}, {2, 2.5}, "wood", false});

    apt.addPremise(c1);
    apt.addPremise(r1);

    PathResult res = PathFinder::findShortestPath(apt, "r1", "c1");
    assert(res.found == true);
    assert(res.waypoints.size() == 4);
    assert(res.totalDistance > 0.0);
    std::cout << "[PASS] testPathFinder" << std::endl;
}

void testRobotStep() {
    Apartment apt("RobotApt");
    auto r1 = std::make_shared<Room>("r1", "Box");
    r1->addWall({{0, 0}, {10, 0}});
    r1->addWall({{10, 0}, {10, 10}});
    r1->addWall({{10, 10}, {0, 10}});
    r1->addWall({{0, 10}, {0, 0}});
    apt.addPremise(r1);

    Robot bot({5.0, 5.0}, 0.5, 5);
    bool moved = bot.makeStep(apt);
    assert(moved == true);
    assert(r1->containsPoint(bot.getPosition()) == true);
    std::cout << "[PASS] testRobotStep" << std::endl;
}

int main() {
    std::cout << "Running unit tests..." << std::endl;
    testGeometryAndRoomArea();
    testPathFinder();
    testRobotStep();
    std::cout << "All unit tests passed successfully!" << std::endl;
    return 0;
}