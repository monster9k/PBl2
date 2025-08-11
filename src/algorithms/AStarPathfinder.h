#pragma once
#include "Pathfinder.h"

class AStarPathfinder : public Pathfinder {
public:
    void findPath(int startId, int endId) override;
};
