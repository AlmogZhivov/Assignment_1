#include "../include/LimitedDriverVolunteer.h"


    LimitedDriverVolunteer::LimitedDriverVolunteer
        (int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) : 
        DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders),
        ordersLeft(maxOrders) {}

    LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const  {
        return new LimitedDriverVolunteer(*this);
    }

    int LimitedDriverVolunteer::getMaxOrders() const {
        return maxOrders;
    }

    int LimitedDriverVolunteer::getNumOrdersLeft() const {
        return ordersLeft;
    }

    bool LimitedDriverVolunteer::hasOrdersLeft() const  {
        return ordersLeft > 0;
    }

    bool LimitedDriverVolunteer::canTakeOrder(const Order& order) const  {
        return hasOrdersLeft() && canTakeOrder(order);
    }

    void LimitedDriverVolunteer::acceptOrder(const Order& order){
        ordersLeft = ordersLeft - 1;
        DriverVolunteer::acceptOrder(order);
    }

    string LimitedDriverVolunteer::toString() const  {
        std::string output = "";

        output += "volunteerID: " + std::to_string(this->getId()) + "\n";

        if (isBusy())
        {
            output += "isBusy: True\n";
        }
        else
        {
            output += "isBusy: False\n";
        }

        output += "distanceLeft: " + std::to_string(this->getDistanceLeft()) + "\n";
        output += "ordersLeft: " + std::to_string(ordersLeft) + "\n";

        return output;
    }

    //LimitedDriverVolunteer::~LimitedDriverVolunteer() {}