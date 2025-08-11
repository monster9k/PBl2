#pragma once
#include "../core/Order.h"

class Dispatcher {
public:
    virtual ~Dispatcher() {}
    virtual void assignOrders() = 0;
};
