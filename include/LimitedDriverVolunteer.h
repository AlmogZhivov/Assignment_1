#pragma once
#include <string>
#include <vector>
#include "../include/Order.h"
#include "../include/Volunteer.h"
#include "../include/DriverVolunteer.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class LimitedDriverVolunteer : public DriverVolunteer
{

public:
    LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders);
    LimitedDriverVolunteer *clone() const override;
    int getMaxOrders() const;
    int getNumOrdersLeft() const;
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy, the order is within the maxDistance.
    void acceptOrder(const Order &order) override;        // Assign distanceLeft to order's distance and decrease ordersLeft
    string toString() const override;


private:
    const int maxOrders; // The number of orders the volunteer can process in the whole simulation
    int ordersLeft;      // The number of orders the volunteer can still take
};