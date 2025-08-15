#include "Node.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <fstream>

using namespace std;

Node::Node() : id(""), pos({0, 0}), type("normal") {}

Node::Node(string id, int x, int y, string type)
    : id(id), pos({x, y}), type(type) {}

// get
string Node::getId() const
{
    return this->id;
}

pair<int, int> Node::getPos() const
{
    return this->pos;
}

string Node::getType() const
{
    return this->type;
}

// set

void Node::setId(string id)
{
    this->id = id;
}

void Node::setPos(int x, int y)
{
    this->pos = {x, y};
}

void Node::setType(string type)
{
    this->type = type;
}

void Node::addNeighbor(Node *neighbor, float cost)
{
    neighbors.push_back({neighbor, cost});
}

const vector<Node::Neighbor> &Node::getNeighbors() const
{
    return neighbors;
}

NodeData readAllNodeFromFile(const string &filename)
{
    ifstream file(filename);
    NodeData data;

    if (!file)
    {
        cerr << "Error: Cannot open file\n";
        return {};
    }
    string id, type;
    int x, y;

    while (file >> id >> x >> y >> type)
    {
        Node *newNode = new Node(id, x, y, type);
        data.allNodes.push_back(newNode);
        data.nodeMap[id] = newNode;
    }
    file.close();
    return data;
}

Node::~Node() {}
