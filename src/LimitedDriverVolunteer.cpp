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
        return hasOrdersLeft() && DriverVolunteer::canTakeOrder(order);
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
            output += "OrderID: " + std::to_string(activeOrderId) + "\n";
            output += "distance_Left: " + std::to_string(getDistanceLeft()) + "\n";
        }
        else
        {
            output += "isBusy: False\n";
            output += "OrderID: None\n";
            output += "distance_Left: None\n";
        }


        output += "ordersLeft: " + std::to_string(ordersLeft) + "\n";

        return output;
    }

