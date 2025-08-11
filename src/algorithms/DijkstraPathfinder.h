#pragma once
#include "Pathfinder.h"

class DijkstraPathfinder : public Pathfinder {
public:
    void findPath(int startId, int endId) override;
};
