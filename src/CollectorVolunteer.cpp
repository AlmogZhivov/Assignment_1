#include "../include/CollectorVolunteer.h"


    //CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) :
      //  Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}
    CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown)
    {
    }
    int CollectorVolunteer::getCoolDown() const {
        return coolDown;
    }

    int CollectorVolunteer::getTimeLeft() const {
        return timeLeft;
    }

    bool CollectorVolunteer::decreaseCoolDown() {
        if (timeLeft > 0) {
            timeLeft = timeLeft - 1;
        }

        return timeLeft == 0;
    }

    bool CollectorVolunteer::hasOrdersLeft() const { 
        return true;
    }

    bool CollectorVolunteer::canTakeOrder(const Order& order) const  { 
        return timeLeft == 0 && !isBusy() &&
        order.getStatus() == OrderStatus::PENDING;
    }

    void CollectorVolunteer::acceptOrder(const Order& order)  { 
        activeOrderId = order.getId();
        completedOrderId = NO_ORDER;
        timeLeft = coolDown;
    }

    string CollectorVolunteer::toString() const  {
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

        output += "timeLeft: " + std::to_string(timeLeft) + "\n";
        output += "ordersLeft: No Limit\n";

        return output;
    }


    void CollectorVolunteer::step() {
        if(decreaseCoolDown()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }

    CollectorVolunteer::~CollectorVolunteer() {}