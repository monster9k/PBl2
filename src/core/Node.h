#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

using namespace std;

class Node
{
protected:
    string id;
    pair<float, float> pos;
    string type;

    struct Neighbor
    {
        Node *node;
        float cost;
    }; // check các node kề
    vector<Neighbor> neighbors;

public:
    Node();
    Node(string id, float x, float y, string type);
    // get
    string getId() const;
    pair<float, float> getPos() const;
    string getType() const;

    // set
    void setId(string id);
    void setPos(float x, float y);
    void setType(string type);

    // graph : (sau neu co dung)
    void addNeighbor(Node *neighbor, float cost);
    const vector<Neighbor> &getNeighbors() const;

    void clearNeighbors();

    ~Node();
    // TODO: Thuộc tính & phương thức node
};

struct NodeData
{
    vector<Node *> allNodes;
    unordered_map<string, Node *> nodeMap;
};

NodeData readAllNodeFromFile(const string &filename);
