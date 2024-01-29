#include "Customer.h"

class Customer {

    Customer::Customer(int id, const string& name, int locationDistance, int maxOrders) :
        id(id), name(name), locationDistance(locationDistance),
        maxOrders(maxOrders) {
            vector<int> ordersId;
        }

    const string& Customer::getNmae() const{
        return name;
    }


    int Customer::getId() const
    {
        return id;
    }

    int Customer::getCustomerDistance() const {
        return locationDistance;
    }

    int Customer::getMaxOrders() const {
        return maxOrders;
    }

    const Customer::vector<int>& getOrdersIds() const{
        return ordersIds
    }

    int Customer::getNumOrders() const {
        return ordersIds.size();
    }

    bool Customer::canMakeOrder() const {
        return ordersIds.size() < maxOrders;
    }

    int Customer::addOrder(int orderId) {
        if (canMakeOrder()) {
            ordersIds.push_back(orderId);
            return orderId;
        }

        return -1;
    }

}