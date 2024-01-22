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
    int distancePerStep;  // For drivers only
    int maxOrders;        // Optional field
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
                    std::cout << "Name: " << customer.name << "\n";
                    std::cout << "Type: " << customer.type << "\n";
                    std::cout << "Distance: " << customer.distance << "\n";
                    std::cout << "Max Orders: " << customer.maxOrders << "\n\n";
                }
                customerCounter++;
            }
            else {
                std::cerr << "Unknown keyword: " << keyword << "\n";
            }  
        }
    }

    configFile.close();
}