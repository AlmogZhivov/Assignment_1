#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse();
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addCustomer(Customer* customer);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        vector<string> split(string s, string delimiter);
        vector<Order*> &getPendingOrders();
        vector<Order*> &getInProcessOrders();
        vector<Order*> &getCompletedOrders();
        vector<Volunteer*> &getVolunteers();
        const int getCustomerCounter() const;
        const int getOrderCounter() const;
        bool customerExists(int customerId) const;
        bool volunteerExists(int volunteerId) const;
        bool orderExists(int orderId) const;

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;
};