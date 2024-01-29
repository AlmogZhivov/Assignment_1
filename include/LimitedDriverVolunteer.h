#pragma once
#include <string>
#include <vector>
#include "Order.h"
#include "Volunteer.h"
#include "DriverVolunteer.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class LimitedDriverVolunteer : public DriverVolunteer
{

public:
    (int id, const string &name, int coolDown, int maxOrders);
    LimitedDriverVolunteer *clone() const override;
    int getMaxOrders() const;
    int getNumOrdersLeft() const;
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy, the order is within the maxDistance.
    void acceptOrder(const Order &order) override;        // Assign distanceLeft to order's distance and decrease ordersLeft
    string toString() const override;

    ~LimitedDriverVolunteer() override;

private:
    const int maxOrders; // The number of orders the volunteer can process in the whole simulation
    int ordersLeft;      // The number of orders the volunteer can still take
};