#pragma once
#include <string>
#include <vector>

#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
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
        const int getCustomerCounter() const;
        const int getOrderCounter() const;
        bool customerExists(int customerId) const;
        bool volunteerExists(int volunteerId) const;
        bool orderExists(int orderId) const;
        void simulateStep(int numOfSteps);
        string stringOrdersWhenClose() const;
        
        // rule of 3
        ~WareHouse();
        WareHouse(const WareHouse& other);
        WareHouse& operator=(const WareHouse& other);
        //rule of 5
        WareHouse(WareHouse&& other);
        WareHouse& operator=(WareHouse&& other);

    private:
        bool isOpen;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;
        
        Order* defaultOrder;
        Customer* defaultCustomer;
        Volunteer* defaultVolunteer;
        
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
};