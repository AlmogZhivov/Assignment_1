#include "../include/DriverVolunteer.h"


    DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) :
    Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep),
    distanceLeft(0) {}

    DriverVolunteer* DriverVolunteer::clone() const  {
        return new DriverVolunteer(*this);
    }

    int DriverVolunteer::getDistanceLeft() const {
        return distanceLeft;
    }

    int DriverVolunteer::getMaxDistance() const {
        return maxDistance;
    }

    int DriverVolunteer::getDistancePerStep() const {
        return distancePerStep;
    }

    bool DriverVolunteer::decreaseDistanceLeft(){
        distanceLeft = distanceLeft - distancePerStep;

        if (this->getDistanceLeft() <= 0) {
            distanceLeft = 0;
            return true;
        }

        return false;
    }

    bool DriverVolunteer::hasOrdersLeft() const  {
        return true;
    }

    bool DriverVolunteer::canTakeOrder(const Order& order) const {
        return order.getDistance()<=maxDistance && !isBusy() && 
        order.getStatus() == OrderStatus::COLLECTING;
    }

    void DriverVolunteer::acceptOrder(const Order& order)  {
        activeOrderId = order.getId();
        distanceLeft = order.getDistance();
    }

    string DriverVolunteer::toString() const  {
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

        output += "distanceLeft: " + std::to_string(this->getDistanceLeft()) + "\n";
        output += "ordersLeft: No Limit\n";

        return output;
    }


    void DriverVolunteer::step()  {
        // in the decreaseDistanceLeft method it performs the "driving"
        if (decreaseDistanceLeft()) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }

    DriverVolunteer::~DriverVolunteer() {}
