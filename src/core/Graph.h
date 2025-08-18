#pragma once
#include <vector>
#include "Node.h"
#include "Edge.h"

float calcDistance(pair<float, float> p1, pair<float, float> p2);

void addNodeAndEdges(float mouseX, float mouseY,
                     NodeData &nodeData,
                     vector<Edge *> &edges,
                     const string &nodeFile,
                     const string &edgeFile);

void rebuildNeighbors(vector<Edge *> &edges, unordered_map<string, Node *> &nodeMap);