#include "Edge.h"
#include <fstream>
#include <string>

using namespace std;

Edge::Edge() : from(nullptr), to(nullptr), cost(0), speed(0), active(true) {}

Edge::Edge(Node *from, Node *to, float cost, float speed, bool active)
    : from(from), to(to), cost(cost), speed(speed), active(active) {}

// get
Node *Edge::getFrom() const
{
    return from;
}

Node *Edge::getTo() const
{
    return to;
}

float Edge::getCost() const
{
    return cost;
}

float Edge::getSpeed() const
{
    return speed;
}

bool Edge::isActive() const
{
    return active;
}

// set
void Edge::setCost(float cost)
{
    this->cost = cost;
}

void Edge::setSpeed(float speed)
{
    this->speed = speed;
}

void Edge::setActive(bool status)
{
    this->active = status;
}

// get pos from node

pair<int, int> Edge::getFromPos() const
{
    return from ? from->getPos() : make_pair(0, 0);
}

pair<int, int> Edge::getToPos() const
{
    return to ? to->getPos() : make_pair(0, 0);
}

Edge::~Edge() {}

vector<Edge *> readAllEdgeFromFile(
    const string &filename,
    const unordered_map<string, Node *> &nodeMap)
{
    ifstream file(filename);
    vector<Edge *> allEdges;

    if (!file)
    {
        cerr << "Error: Cannot open file\n";
        return {};
    }

    string fromId, toId;
    float cost, speed;
    bool active;

    while (file >> fromId >> toId >> cost >> speed >> active)
    {
        auto fromIt = nodeMap.find(fromId);
        auto toIt = nodeMap.find(toId);

        if (fromIt != nodeMap.end() && toIt != nodeMap.end())
        {
            Edge *e = new Edge(fromIt->second, toIt->second, cost, speed, active);
            allEdges.push_back(e);

            // cập nhật neighbor
            fromIt->second->addNeighbor(toIt->second, cost);
            toIt->second->addNeighbor(fromIt->second, cost); // nếu 2 chiều
        }
    }
    return allEdges;
}
