#include "../include/Order.h"


    Order::Order(int id, int customerId, int distance) :
        id(id), customerId(customerId), distance(distance) {
            collectorId = NO_VOLUNTEER;
            driverId = NO_VOLUNTEER;
        }


    int Order::getId() const {
        return id;
    }

    int Order::getCustomerId() const {
        return customerId;
    }

    void Order::setStatus(OrderStatus status){
        this->status = status;
    }

    void Order::setCollectorId(int collectorId) {
        this->collectorId = collectorId;
    }

    void Order::setDriverId(int driverId) {
        this->driverId = driverId;
    }

    int Order::getCollectorId() const {
        return collectorId;
    }

    int Order::getDriverId() const {
        return driverId;
    }
    int Order::getDistance() const {
        return distance;
    }

    OrderStatus Order::getStatus() const {
        return status;
    }

    const string Order::getStringStatus() const {

        // maybe should use equals() for the OrderStatus objects?

        if (status == OrderStatus::PENDING) {
            return "Pending";
        }
        else if (status == OrderStatus::COLLECTING){
            return "Collecting";
        }
        else if (status == OrderStatus::DELIVERING){
            return "Delivering";
        }
        else {
            // status is COMLETED
            return "Completed";
        }

    }

    const string Order::toString() const 
    {
        std::string output = "";

        output += "OrderId: " + std::to_string(id) + "\n";
        output += "OrderStatus: " + getStringStatus() + "\n";
        output += "CustomerID: " + std::to_string(customerId) + "\n";

        if (getCollectorId() == NO_VOLUNTEER)
        {
            output += "Collector: None\n";
        }
        else
        {
            output += "Collector:" + std::to_string(collectorId) +  "\n";
        }

        if (getDriverId() == NO_VOLUNTEER){
            output += "Driver: None\n";
        }
        else {
            output += "Driver: " + std::to_string(driverId) + "\n";
        }

        return output;
    }