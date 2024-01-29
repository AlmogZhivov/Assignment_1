#include "DriverVolunteer.h"

class DriverVolunteer : public Volunteer {

    DriverVolunteer::DriverVolunteer
    (int id, const string &name, int maxDistance, int distancePerStep) :
    Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep),
    distanceLeft(0) {}

    DriverVolunteer* DriverVolunteer::clone() const override {
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

        if (distacneLeft <= 0) {
            distanceLeft = 0;
            return true;
        }

        return false;
    }

    bool DriverVolunteer::hasOrdersLeft() const override {
        return true;
    }

    bool DriverVolunteer::canTakeOrder(const Order& order) const {
        return order.getDistance<=maxDistance && !isBusy() && 
        order.getStatus() == OrderStatus::COLLECTING;
    }

    void DriverVolunteer::acceptOrder(const Order& order) override {
        activeOrderId = order.getId();
        distanceLeft = order.getDistance();
    }

    string DriverVolunteer::toString() const override {
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
        output += "ordersLeft: No Limit\n";

        return output;
    }


    void DriverVolunteer::step() override {
        // in the decreaseDistanceLeft method it performs the "driving"
        if (decreaseDistanceLeft()) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }

    DriverVolunteer::~DriverVolunteer() {}


}