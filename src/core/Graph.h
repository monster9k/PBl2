#pragma once
#include <vector>
#include "Node.h"
#include "Edge.h"

class Graph {
public:
    Graph();
    ~Graph();

    // TODO: Các phương thức quản lý đồ thị
    void addNode(const Node& node);
    void addEdge(const Edge& edge);
};
