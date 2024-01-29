#include CollectorVolunteer

class CollectorVolunteer : public Volunteer {

    CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) :
        Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

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

    bool CollectorVolunteer::hasOrdersLeft() const override { 
        return true;
    }

    bool CollectorVolunteer::canTakeOrder(const Order& order) const override { 
        return timeLeft == 0 && !isBusy() &&
        order.getStatus() == OrderStatus::PENDING;
    }

    void CollectorVolunteer::acceptOrder(const Order& order) override { 
        activeOrderId = order.getId();
        completedOrderId = NO_ORDER;
        timeLeft = coolDown;
    }

    string CollectorVolunteer::toString() const override {
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


    void CollectorVolunteer::step() override {
        if(decreaseCoolDown()){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }

    CollectorVolunteer::~CollectorVolunteer() {}
}