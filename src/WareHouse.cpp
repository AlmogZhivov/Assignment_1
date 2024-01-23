#include "../include/WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/Action.h"
#include "../include/Volunteer.h"
using namespace std;

WareHouse::WareHouse() : isOpen(false), customers(), volunteers(), actionsLog(), pendingOrders(), inProcessOrders(), completedOrders() 
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
    while (inputString != "closeall")
    {

    }
}