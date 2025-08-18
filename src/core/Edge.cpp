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

pair<float, float> Edge::getFromPos() const
{
    return from ? from->getPos() : make_pair(0.0f, 0.0f);
}

pair<float, float> Edge::getToPos() const
{
    return to ? to->getPos() : make_pair(0.0f, 0.0f);
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

void updateEdgeFile(const string &filename, const vector<Edge *> &edges)
{
    ofstream out(filename);
    if (!out.is_open())
        return;

    for (auto &e : edges)
    {
        out << e->getFrom()->getId() << " "
            << e->getTo()->getId() << " "
            << e->getCost() << " "
            << e->getSpeed() << " "
            << e->isActive() << "\n"; // cập nhật trạng thái
    }
    out.close();
}
