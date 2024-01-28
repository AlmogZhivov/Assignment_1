#include "../include/WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/Action.h"
#include "../include/Volunteer.h"
using namespace std;

WareHouse::WareHouse() : isOpen(false)
{
    customerCounter = 0;
    volunteerCounter = 0;
}
struct CustomerConfig {
    std::string name;
    std::string type;
    int distance;
    int maxOrders;
};
struct VolunteerConfig {
    std::string name;
    std::string role;
    int coolDown;
    int maxDistance;
    int distancePerStep; 
    int maxOrders;        
};

WareHouse::WareHouse(const string& configFilePath)
{
    isOpen = true;
    customerCounter = 0;
    volunteerCounter = 0;
    string line;
    bool inputNext = false;
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Unable to open the configuration file.\n";
    }
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string keyword;
        if (line != "" && line.at(0) == '#') {
            inputNext = true;
        }
        else if (inputNext && iss >> keyword) {
            if (keyword == "customer") {
                CustomerConfig customer;
                if (iss >> customer.name >> customer.type >> customer.distance >> customer.maxOrders) {
                    if (customer.type == "soldier") {
                        customers.push_back(new SoldierCustomer(customerCounter, customer.name, customer.distance, customer.maxOrders));
                    }
                    else if (customer.type == "civilian") {
                        customers.push_back(new CivilianCustomer(customerCounter, customer.name, customer.distance, customer.maxOrders));
                    }
                }
                customerCounter++;
            }
            else if (keyword == "volunteer") {
                VolunteerConfig volunteer;
                if (iss >> volunteer.name >> volunteer.role) {
                    if (volunteer.role == "collector" && iss >> volunteer.coolDown) {
                        volunteers.push_back(new CollectorVolunteer(volunteerCounter, volunteer.name, volunteer.coolDown));
                    }
                    else if (volunteer.role == "limited_collector" && iss >> volunteer.coolDown >> volunteer.maxOrders) {
                        volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, volunteer.name, volunteer.coolDown, volunteer.maxOrders));
                    }
                    else if (volunteer.role == "driver" && iss >> volunteer.maxDistance >> volunteer.distancePerStep) {
                        volunteers.push_back(new DriverVolunteer(volunteerCounter, volunteer.name, volunteer.maxDistance, volunteer.distancePerStep));
                    } 
                    else if (volunteer.role == "limited_driver" && iss >> volunteer.maxDistance >> volunteer.distancePerStep >> volunteer.maxOrders) {
                        volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, volunteer.name, volunteer.maxDistance, volunteer.distancePerStep, volunteer.maxOrders));
                    } 
                    else {
                        std::cerr << "Error parsing volunteer configuration.\n";
                    }
                }
                volunteerCounter++;
            }  
        }
    }
    configFile.close();
}
void WareHouse::start()
{
    cout << "WareHouse is open!" << endl;
    string inputString;
    getline(cin, inputString);
    bool isClosed = false;
    while (!isClosed)
    {
        string command = inputString.substr(0, inputString.find_first_of(' '));
        vector<string> vecOfInput = split(inputString, " ");
        if (command == "step")
        {
            SimulateStep stepAction(stoi(vecOfInput.at(1)));
            stepAction.setActionString(inputString);
            stepAction.act(*this);
        }
        else if (command == "order")
        {
            AddOrder orderAction(stoi(vecOfInput.at(1)));
            orderAction.setActionString(inputString);
            orderAction.act(*this);
        }
        else if (command == "customer")
        {
            AddCustomer customerAction(vecOfInput.at(1), vecOfInput.at(2), stoi(vecOfInput.at(3)), stoi(vecOfInput.at(4)));
            customerAction.setActionString(inputString);
            customerAction.act(*this);
        }
        else if (command == "orderStatus")
        {
            PrintOrderStatus orderStatusAction(stoi(vecOfInput.at(1)));
            orderStatusAction.setActionString(inputString);
            orderStatusAction.act(*this);
        }
        else if (command == "customerStatus")
        {
            PrintCustomerStatus customerStatusAction(stoi(vecOfInput.at(1)));
            customerStatusAction.setActionString(inputString);
            customerStatusAction.act(*this);
        }
        else if (command == "volunteerStatus")
        {
            PrintVolunteerStatus volunteerStatusAction(stoi(vecOfInput.at(1)));
            volunteerStatusAction.setActionString(inputString);
            volunteerStatusAction.act(*this);
        }
        else if (command == "log")
        {
            PrintActionsLog logAction;
            logAction.setActionString(inputString);
            logAction.act(*this);
        }
        else if (command == "close")
        {
            Close closeAction;
            closeAction.setActionString(inputString);
            closeAction.act(*this);
            isClosed = true;

        }
        else if (command == "backup")
        {
            BackupWareHouse backupAction;
            backupAction.setActionString(inputString);
            backupAction.act(*this);
        }
        else if (command == "restore")
        {
            RestoreWareHouse restoreAction;
            restoreAction.setActionString(inputString);
            restoreAction.act(*this);
        }
        if (!isClosed) {
            getline(cin, inputString);
        }
    }
    isOpen = false;
}
vector<string> WareHouse::split(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
void WareHouse::addAction(BaseAction* action)
{
    actionsLog.push_back(action);
}
void WareHouse::addCustomer(Customer* customer)
{
    customers.push_back(customer);
}
const std::vector<BaseAction*>& WareHouse::getActions() const
{
    return actionsLog;
}
const int WareHouse::getCustomerCounter() const
{
    return customerCounter;
}
const int WareHouse::getOrderCounter() const
{
    return orderCounter;
}
std::vector<Order*>& WareHouse::getPendingOrders()
{
    return pendingOrders;
}
vector<Order*>& WareHouse::getInProcessOrders()
{
    return inProcessOrders;
}
vector<Order*>& WareHouse::getCompletedOrders()
{
    return completedOrders;
}
vector<Volunteer*>& WareHouse::getVolunteers()
{
    return volunteers;
}
bool WareHouse::customerExists(int customerId) const
{
    return customerId <= customerCounter;
}
bool WareHouse::orderExists(int orderId) const
{
    return orderId <= orderCounter;
}
bool WareHouse::volunteerExists(int volunteerId) const
{
    return volunteerId <= volunteerCounter;
}