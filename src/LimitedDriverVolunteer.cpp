#include "LimitedDriverVolunteer.h"

class LimitedDriverVolunteer : public DriverVolunteer {

    LimitedDriverVolunteer::LimitedDriverVolunteer
        (int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : 
        DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders),
        ordersLeft(maxOrders) {}

    LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const override {
        return new LimitedDriverVolunteer(*this);
    }

    int LimitedDriverVolunteer::getMaxOrders() const {
        return maxOrders;
    }

    int LimitedDriverVolunteer::getNumOrdersLeft() const {
        return ordersLeft;
    }

    bool LimitedDriverVolunteer::hasOrdersLeft() const override {
        return ordersLeft > 0;
    }

    bool canTakeOrder(const Order& order) const override {
        return hasOrdersLeft() && DriverVolunteer.canTakeOrder(order);
    }

    void LimitedDriverVolunteer::acceptOrder(const Order& order){
        ordersLeft = ordersLeft - 1;
        DriverVolunteer::acceptOrder(order);
    }

    string LimitedDriverVolunteer::toString() const override {
        std::string output = "";

        output += "volunteerID: " + std::to_string(id) + "\n";

        if (isBusy)
        {
            output += "isBusy: True\n";
        }
        else
        {
            output += "isBusy: False\n";
        }

        output += "timeLeft: " + std::to_string(timeLeft) + "\n";
        output += "ordersLeft: " + std::to_string(ordersLeft) + "\n";

        return output;
    }

    LimitedDriverVolunteer::~LimitedDriverVolunteer() {}
}