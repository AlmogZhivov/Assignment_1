#include "LimitedCollectorVolunteer.h"

class LimitedCollectorVolunteer : public CollectorVolunteer{

    LimitedCollectorVolunteer::LimitedCollectorVolunteer
    (int id, const string &name, int coolDown, int maxOrders) : 
    CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

    LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const override{
        return new LimitedCollectorVolunteer(*this);
    }

    bool LimitedCollectorVolunteer::hasOrdersLeft() const override{
        return ordersLeft > 0;
    }

    bool LimitedCollectorVolunteer::canTakeOrder(const Order& order) const override {
        return hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order);
    }

    void LimitedCollectorVolunteer::acceptOrder(const Order& order) override {
        ordersLeft = ordersLeft - 1;
        CollectorVolunteer::acceptOrder(order);
    }

    int LimitedCollectorVolunteer::getMaxOrders() const {
        return maxOrders;
    }

    int LimitedCollectorVolunteer::getNumOrdersLeft() const {
        return ordersLeft;
    }

    string LimitedCollectorVolunteer::toString() const override
    {
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

    LimitedCollectorVolunteer::~LimitedCollectorVolunteer() {}

}