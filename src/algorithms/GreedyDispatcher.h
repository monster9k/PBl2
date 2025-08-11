#pragma once
#include "Dispatcher.h"

class GreedyDispatcher : public Dispatcher {
public:
    void assignOrders() override;
};
