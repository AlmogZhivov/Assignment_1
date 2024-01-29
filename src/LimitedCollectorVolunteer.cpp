#include "../include/LimitedCollectorVolunteer.h"

    LimitedCollectorVolunteer::LimitedCollectorVolunteer
    (int id, const string &name, int coolDown, int maxOrders) : 
    CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

    LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
        return new LimitedCollectorVolunteer(*this);
    }

    bool LimitedCollectorVolunteer::hasOrdersLeft() const {
        return ordersLeft > 0;
    }

    bool LimitedCollectorVolunteer::canTakeOrder(const Order& order) const  {
        return CollectorVolunteer::hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order);
    }

    void LimitedCollectorVolunteer::acceptOrder(const Order& order)  {
        ordersLeft = ordersLeft - 1;
        CollectorVolunteer::acceptOrder(order);
    }

    int LimitedCollectorVolunteer::getMaxOrders() const {
        return maxOrders;
    }

    int LimitedCollectorVolunteer::getNumOrdersLeft() const {
        return ordersLeft;
    }

    string LimitedCollectorVolunteer::toString() const 
    {
        std::string output = "";

        output += "volunteerID: " + std::to_string(this->getId()) + "\n";

        if (isBusy())
        {
            output += "isBusy: True\n";
            output += "OrderID: " + activeOrderId;
        }
        else
        {
            output += "isBusy: False\n";
            output += "OrderID: None";
        }

        output += "timeLeft: " + std::to_string(this->getTimeLeft()) + "\n";
        output += "ordersLeft: " + std::to_string(ordersLeft) + "\n";

        return output;
    }

    //LimitedCollectorVolunteer::~LimitedCollectorVolunteer() {}
