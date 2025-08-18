#pragma once
#include "Pathfinder.h"
#include "D:/SDL2-project/src/core/Node.h"
#include "D:/SDL2-project/src/core/Edge.h"
#include <unordered_map>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

vector<Node *> dijkstra(Node *start, Node *goal);
