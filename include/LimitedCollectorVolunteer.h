#pragma once
#include <string>
#include <vector>
#include "../include/Order.h"
#include "../include/CollectorVolunteer.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class LimitedCollectorVolunteer : public CollectorVolunteer
{

public:
    LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders);
    LimitedCollectorVolunteer *clone() const override;
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order &order) const override;
    void acceptOrder(const Order &order) override;

    int getMaxOrders() const;
    int getNumOrdersLeft() const;
    string toString() const override;

    //~LimitedCollectorVolunteer();

private:
    const int maxOrders; // The number of orders the volunteer can process in the whole simulation
    int ordersLeft;      // The number of orders the volunteer can still take
};