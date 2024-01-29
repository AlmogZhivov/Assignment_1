#pragma once
#include <string>
#include <vector>
#include "Order.h"
#include "Volunteer.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class CollectorVolunteer : public Volunteer
{

public:
    CollectorVolunteer(int id, const string &name, int coolDown);
    CollectorVolunteer *clone() const override;
    void step() override;
    int getCoolDown() const;
    int getTimeLeft() const;
    bool decreaseCoolDown(); // Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order &order) const override;
    void acceptOrder(const Order &order) override;
    string toString() const override;

    ~CollectorVolunteer() override;

private:
    const int coolDown; // The time it takes the volunteer to process an order
    int timeLeft;       // Time left until the volunteer finishes his current order
};
