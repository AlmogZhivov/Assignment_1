#include "WareHouse.h"

class WareHouse {

    void WareHouse::addOrder(Order* order){
        if (order != nullptr){
            pendingOrders.push_back(order);
            orderCounter = orderCounter + 1;
        }
    }

    // assumes customerId is a valid id
    Customer& WareHouse::getCustomer(int customerId) const {
        Customer output = Customer(NO_VOLUNTEER, "", 1, 1);
        for (Customer* cus : customers){
            if ((cus*).getId == customerId){
                output = cus*;
                return output;
            }
        }
        return output;
    }

    // assumes volunteerId is a valid id
    Volunteer& WareHouse::getVolunteer(int volunteerId) const {
        Volunteer output;
        for (Volunteer* vol : volunteers){
            if ((vol*).getId == volunteerId){
                output = vol*;
                return output;
            }
        }
        return output;
    }


    // assumes order exists
    Order& WareHouse::getOrder(int orderId){
        Order output;
        for(Order* order : pendingOrders){
            if ((order*).getId == orderId){
                output = order*;
                return output;
            }
        }
        for (Order *order : inProcessOrders)
        {
            if ((order *).getId == orderId)
            {
                output = order *;
                return output;
            }
        }
        for (Order *order : completedOrders)
        {
            if ((order *).getId == orderId)
            {
                output = order *;
                return output;
            }
        }
        return output;
    }

    void WareHouse::close() {
        this.isOpen = false;
    }

    void WareHouse::open() {
        this.isOpen = true;
    }

    WareHouse::~WareHouse() {
        for (Order* order : pendingOrders){
            delete order;
        }
        for (Order* order : inProcessOrders){
            delete order;
        }
        for (Order* order : completedOrder) {
            delete order;
        }
        for (Volunteer* vol : volunteers) {
            delete vol;
        }
        for (Customer* cus : customers) {
            delete cus;
        }
        for (BaseAction* act : actionsLog){
            delete act;
        }
    }
}