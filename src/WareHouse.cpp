#include "../include/WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/Action.h"
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
    int volunteerID = 0;
    int orderID = 0;
    string line;
    int inputType = 1;
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
                    // Additional fields for drivers
                    if (volunteer.role == "collector" && iss >> volunteer.coolDown) {
                        std::cout << "Name: " << volunteer.name << "\n";
                        std::cout << "Role: " << volunteer.role << "\n";
                        std::cout << "Cool Down: " << volunteer.coolDown << "\n";
                    }
                    else if (volunteer.role == "limited_collector" && iss >> volunteer.coolDown >> volunteer.maxOrders) {
                        std::cout << "Name: " << volunteer.name << "\n";
                        std::cout << "Role: " << volunteer.role << "\n";
                        std::cout << "Cool Down: " << volunteer.coolDown << "\n";
                        std::cout << "Max Orders: " << volunteer.maxOrders << "\n";
                    }
                    else if (volunteer.role == "driver" && iss >> volunteer.maxDistance >> volunteer.distancePerStep) {
                        std::cout << "Name: " << volunteer.name << "\n";
                        std::cout << "Role: " << volunteer.role << "\n";
                        std::cout << "Max Distance: " << volunteer.maxDistance << "\n";
                        std::cout << "Distance Per Step: " << volunteer.distancePerStep << "\n";
                    } 
                    else if (volunteer.role == "limited_driver" && iss >> volunteer.maxDistance >> volunteer.distancePerStep >> volunteer.maxOrders) {
                        std::cout << "Name: " << volunteer.name << "\n";
                        std::cout << "Role: " << volunteer.role << "\n";
                        std::cout << "Max Distance: " << volunteer.maxDistance << "\n";
                        std::cout << "Distance Per Step: " << volunteer.distancePerStep << "\n";
                        std::cout << "Max Orders: " << volunteer.maxOrders << "\n\n";
                    } 
                    else {
                        std::cerr << "Error parsing volunteer configuration.\n";
                    }
                }
            }  
        }
    }

    configFile.close();
}