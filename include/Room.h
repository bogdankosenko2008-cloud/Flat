#pragma once
#include "Geometry.h"
#include <vector>
#include <string>
#include <memory>

class Premise {
protected:
    std::string id;
    std::string name;
    std::vector<Wall> walls;
    std::vector<Door> doors;

public:
    Premise(const std::string& id, const std::string& name) : id(id), name(name) {}
    virtual ~Premise() = default;

    void addWall(const Wall& w) { walls.push_back(w); }
    void addDoor(const Door& d) { doors.push_back(d); }

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::vector<Wall>& getWalls() const { return walls; }
    const std::vector<Door>& getDoors() const { return doors; }

    virtual std::string getType() const = 0;

    double calculateArea() const;
    Point getCenter() const;
    bool containsPoint(const Point& p) const;
};

class Room : public Premise {
public:
    Room(const std::string& id, const std::string& name) : Premise(id, name) {}
    std::string getType() const override { return "room"; }
};

class Corridor : public Premise {
public:
    Corridor(const std::string& id, const std::string& name) : Premise(id, name) {}
    std::string getType() const override { return "corridor"; }
};