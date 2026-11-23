#include "PathFinder.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <cmath>

namespace {
    struct Node {
        int id;
        Point pos;
        std::string premiseId;
    };

    struct Edge {
        int to;
        double weight;
    };

    bool pointsClose(const Point& a, const Point& b, double eps = 0.25) {
        return a.distanceTo(b) < eps;
    }
}

PathResult PathFinder::findShortestPath(
    const Apartment& apartment,
    const std::string& startPremiseId,
    const std::string& targetPremiseId
) {
    PathResult result;
    const auto& premises = apartment.getPremises();
    if (premises.empty()) return result;

    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> adj;

    auto addNode = [&](const Point& p, const std::string& pId) -> int {
        int id = static_cast<int>(nodes.size());
        nodes.push_back({id, p, pId});
        adj.emplace_back();
        return id;
    };

    auto addEdge = [&](int u, int v) {
        double w = nodes[u].pos.distanceTo(nodes[v].pos);
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    };

    int startNode = -1;
    int targetNode = -1;

    // 1. Створюємо вузли для центрів приміщень та їхніх дверей
    std::vector<std::vector<int>> premiseDoorNodes(premises.size());

    for (size_t i = 0; i < premises.size(); ++i) {
        int centerId = addNode(premises[i]->getCenter(), premises[i]->getId());
        if (premises[i]->getId() == startPremiseId) startNode = centerId;
        if (premises[i]->getId() == targetPremiseId) targetNode = centerId;

        for (const auto& d : premises[i]->getDoors()) {
            int doorNodeId = addNode(d.getCenter(), premises[i]->getId());
            premiseDoorNodes[i].push_back(doorNodeId);
            addEdge(centerId, doorNodeId);
        }
    }

    if (startNode == -1 || targetNode == -1) return result;

    // 2. З'єднуємо спільні двері між сусідніми приміщеннями
    for (size_t i = 0; i < premises.size(); ++i) {
        for (size_t j = i + 1; j < premises.size(); ++j) {
            for (int dNodeA : premiseDoorNodes[i]) {
                for (int dNodeB : premiseDoorNodes[j]) {
                    if (pointsClose(nodes[dNodeA].pos, nodes[dNodeB].pos)) {
                        addEdge(dNodeA, dNodeB);
                    }
                }
            }
        }
    }

    // 3. Алгоритм Дейкстри для пошуку найкоротшого шляху
    int n = static_cast<int>(nodes.size());
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    std::vector<int> parent(n, -1);
    using PQItem = std::pair<double, int>;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    dist[startNode] = 0.0;
    pq.push({0.0, startNode});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == targetNode) break;

        for (const auto& edge : adj[u]) {
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
                parent[edge.to] = u;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    if (dist[targetNode] == std::numeric_limits<double>::infinity()) {
        return result;
    }

    // Відновлення маршруту
    result.found = true;
    result.totalDistance = dist[targetNode];
    for (int curr = targetNode; curr != -1; curr = parent[curr]) {
        result.waypoints.push_back(nodes[curr].pos);
    }
    std::reverse(result.waypoints.begin(), result.waypoints.end());

    return result;
}