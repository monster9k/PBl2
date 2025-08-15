#pragma once
#include "Node.h"
#include <unordered_map>
using namespace std;

class Edge
{
protected:
    Node *from;
    Node *to;
    float cost;
    float speed; // maybe use
    bool active;

public:
    Edge();
    Edge(Node *from, Node *to, float cost, float speed, bool active);

    // get
    Node *getFrom() const;
    Node *getTo() const;
    float getCost() const;
    float getSpeed() const;
    bool isActive() const;

    // set
    void setFrom(Node *f) { from = f; }
    void setTo(Node *t) { to = t; }
    void setCost(float cost);
    void setSpeed(float speed);
    void setActive(bool status);

    pair<int, int> getFromPos() const;
    pair<int, int> getToPos() const;

    ~Edge();
    // TODO: Thuộc tính & phương thức cạnh
};
vector<Edge *> readAllEdgeFromFile(const string &filename, const unordered_map<string, Node *> &nodeMap);