#pragma once
#include <string>
#include <vector>
#include "../include/Order.h"
#include "../include/Volunteer.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class DriverVolunteer : public Volunteer
{

public:
    DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep);
    DriverVolunteer *clone() const override;

    int getDistanceLeft() const;
    int getMaxDistance() const;
    int getDistancePerStep() const;
    bool decreaseDistanceLeft(); // Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy and the order is within the maxDistance
    void acceptOrder(const Order &order) override;        // Assign distanceLeft to order's distance
    void step() override;                                 // Decrease distanceLeft by distancePerStep
    string toString() const override;

    ~DriverVolunteer();

private:
    const int maxDistance;     // The maximum distance of ANY order the volunteer can take
    const int distancePerStep; // The distance the volunteer does in one step
    int distanceLeft;          // Distance left until the volunteer finishes his current order
};