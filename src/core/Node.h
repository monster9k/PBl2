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
    pair<int, int> pos;
    string type;

    struct Neighbor
    {
        Node *node;
        float cost;
    }; // check các node kề
    vector<Neighbor> neighbors;

public:
    Node();
    Node(string id, int x, int y, string type);
    // get
    string getId() const;
    pair<int, int> getPos() const;
    string getType() const;

    // set
    void setId(string id);
    void setPos(int x, int y);
    void setType(string type);

    // graph : (sau neu co dung)
    void addNeighbor(Node *neighbor, float cost);
    const vector<Neighbor> &getNeighbors() const;
    ~Node();
    // TODO: Thuộc tính & phương thức node
};

struct NodeData
{
    vector<Node *> allNodes;
    unordered_map<string, Node *> nodeMap;
};

NodeData readAllNodeFromFile(const string &filename);
