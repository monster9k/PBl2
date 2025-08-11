#pragma once
#include "../core/Graph.h"

class Pathfinder {
public:
    virtual ~Pathfinder() {}
    virtual void findPath(int startId, int endId) = 0;
};
