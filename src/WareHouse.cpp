#include "../include/WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "../include/Action.h"
#include "../include/Volunteer.h"
#include "../include/CivilianCustomer.h"
#include "../include/SoldierCustomer.h"
#include "../include/CollectorVolunteer.h"
#include "../include/LimitedCollectorVolunteer.h"
#include "../include/DriverVolunteer.h"
#include "../include/LimitedDriverVolunteer.h"
#include "../include/Order.h"
using namespace std;

WareHouse::WareHouse() : isOpen(false)
{
    customerCounter = 0;
    volunteerCounter = 0;
    orderCounter = 0;

    defaultOrder = new Order(-1, NO_VOLUNTEER, NO_VOLUNTEER);
    defaultCustomer = new CivilianCustomer(NO_VOLUNTEER, "Default", 1, 1);
    defaultVolunteer = new LimitedCollectorVolunteer(NO_VOLUNTEER, "Default", 1, 0);
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
    orderCounter = 0;
    defaultOrder = new Order(-1, NO_VOLUNTEER, NO_VOLUNTEER);
    defaultCustomer = new CivilianCustomer(NO_VOLUNTEER, "Default", 1, 1);
    defaultVolunteer = new LimitedCollectorVolunteer(NO_VOLUNTEER, "Default", 1, 0);
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
                        //cout << "mahsan: " + to_string(customerCounter) << endl;
                        //cout << "mahsan: " + customer.name << endl;
                        //cout << "mahsan: " + to_string(customer.distance) << endl;
                        //cout << "mahsan: " + to_string(customer.maxOrders) << endl;
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
            //cout << "insideOrder: " + to_string(stoi(vecOfInput.at(1))) << endl;
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
void WareHouse::simulateStep(int numOfSteps) {
    for (int i = 1; i <= numOfSteps; i++) {
		for (Order* pendingOrder: pendingOrders) {
			for (Volunteer *volunteer: volunteers) {
				if(pendingOrder->getStatus() == OrderStatus::PENDING) {
					if (volunteer->canTakeOrder(*pendingOrder)) {
						pendingOrders.erase(remove_if(pendingOrders.begin(), pendingOrders.end(),
											[pendingOrder](const Order* o) { return o == pendingOrder; }),
								pendingOrders.end());
						if (pendingOrder->getStatus() == OrderStatus::COLLECTING) {
							pendingOrder->setDriverId(volunteer->getId());
							pendingOrder->setStatus(OrderStatus::DELIVERING);
						}
						else if (pendingOrder->getStatus() == OrderStatus::PENDING) {
							pendingOrder->setCollectorId(volunteer->getId());
							pendingOrder->setStatus(OrderStatus::COLLECTING);
						}
						inProcessOrders.push_back(pendingOrder);
					}
				}
			}
		}
		for (Volunteer* volunteer: volunteers) {
			volunteer->step();
		}
		for (Volunteer* volunteer: volunteers) {
			if (volunteer->getCompletedOrderId() != NO_ORDER) {
				for (Order* order: inProcessOrders) {
					if (order->getStatus() == OrderStatus::COLLECTING) {
						pendingOrders.push_back(order);
					}
					else if (order->getStatus() == OrderStatus::DELIVERING) {
						order->setStatus(OrderStatus::COMPLETED);
						completedOrders.push_back(order);
					}
					pendingOrders.erase(remove_if(pendingOrders.begin(), pendingOrders.end(),
											[order](const Order* o) { return o == order; }),
								pendingOrders.end());
				}
			}
		}
		for (Volunteer* volunteer: volunteers) {
			if (!volunteer->hasOrdersLeft()) {
				delete volunteer;
				volunteers.erase(remove_if(volunteers.begin(), volunteers.end(),
											[volunteer](const Volunteer* o) { return o == volunteer; }),
								volunteers.end());
			}
		}
	}
}
string WareHouse::stringOrdersWhenClose() const {
    string output = "";
    for (Order* order : pendingOrders) {
		output += "OrderId: " + to_string(order->getId());
		output += " CustomerId: " + to_string(order->getCustomerId());
		output += " OrderStatus: " + order->getStringStatus();
	}
    for (Order* order : inProcessOrders) {
		output += "OrderId: " + to_string(order->getId());
		output += " CustomerId: " + to_string(order->getCustomerId());
		output += " OrderStatus: " + order->getStringStatus();
	}
	for (Order* order : completedOrders) {
		output += "OrderId: " + to_string(order->getId());
		output += " CustomerId: " + to_string(order->getCustomerId());
		output += " OrderStatus: " + order->getStringStatus();
	}
    return output;
}
void WareHouse::addOrder(Order* order){
    if (order != nullptr){
        pendingOrders.push_back(order);
        orderCounter = orderCounter + 1;
        //cout << "inWarehouse: " + to_string(orderCounter) << endl;
    }
}

// assumes customerId is a valid id
Customer& WareHouse::getCustomer(int customerId) const {
    for (Customer* cus : customers){
        //cout << "here2: " + to_string(cus->getId()) << endl;
	    //cout << "here2: " + cus->getName() << endl;
	    //cout << "here2: " + to_string(cus->getMaxOrders()) << endl;
        if ((*cus).getId() == customerId){
            return *cus;
        }
    }
    return *defaultCustomer;
}

// assumes volunteerId is a valid id
Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (Volunteer* vol : volunteers){
        if ((*vol).getId() == volunteerId){
            return *vol;
        }
    }
    return *defaultVolunteer;
}


// assumes order exists
Order& WareHouse::getOrder(int orderId) const {
    for(Order* order : pendingOrders){
        if ((*order).getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : inProcessOrders)
    {
        if ((*order).getId() == orderId)
        {
            return *order;
        }
    }
    for (Order *order : completedOrders)
    {
        if ((*order).getId() == orderId)
        {
            return *order;
        }
    }
    return *defaultOrder;
}

void WareHouse::close() {
    isOpen = false;
}

void WareHouse::open() {
    isOpen = true;
}

WareHouse::~WareHouse() {
    for (Order* order : pendingOrders){
        delete order;
    }
    for (Order* order : inProcessOrders){
        delete order;
    }
    for (Order* order : completedOrders) {
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

    // deleting defaults
    delete defaultCustomer;
    delete defaultVolunteer;
    delete defaultOrder;
}


WareHouse::WareHouse(const WareHouse& other){
    this->isOpen = other.isOpen;
    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;
    this->orderCounter = other.orderCounter;

    //copying vectors:

    for (BaseAction* act : other.actionsLog){
        this->actionsLog.push_back(act->clone());
    }
    for (Volunteer* vol : other.volunteers){
        this->volunteers.push_back(vol->clone());
    }
    for (Order* order : other.pendingOrders){
        this->pendingOrders.push_back(order->clone());
    }
    for (Order* order : other.inProcessOrders)
    {
        this->inProcessOrders.push_back(order->clone());
    }
    for (Order* order : other.completedOrders)
    {
        this->completedOrders.push_back(order->clone());
    }
    for (Customer* cust : other.customers){
        this->customers.push_back(cust->clone());
    }

    // copying defaults
    this->defaultCustomer = other.defaultCustomer->clone();
    this->defaultVolunteer = other.defaultVolunteer->clone();
    this->defaultOrder = other.defaultOrder->clone();
}

WareHouse::WareHouse(WareHouse &&other) : 
      isOpen(other.isOpen),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)) {

      this->defaultCustomer = other.defaultCustomer->clone();
      this->defaultVolunteer = other.defaultVolunteer->clone();
      this->defaultOrder = other.defaultOrder->clone();

    //nulling ohter's defaults
    other.defaultCustomer = nullptr;
    other.defaultVolunteer = nullptr;
    other.defaultOrder = nullptr;
}
    
WareHouse& WareHouse::operator=(const WareHouse& other){
    if (this == &other) {
        return *this;
    }

    this->isOpen = other.isOpen;
    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;
    this->orderCounter = other.orderCounter;

    // deleting the vectors:
    for (Order *order : pendingOrders)
    {
        delete order;
    }
    for (Order *order : inProcessOrders)
    {
        delete order;
    }
    for (Order *order : completedOrders)
    {
        delete order;
    }
    for (Volunteer *vol : volunteers)
    {
        delete vol;
    }
    for (Customer *cus : customers)
    {
        delete cus;
    }
    for (BaseAction *act : actionsLog)
    {
        delete act;
    }

    //clearing all vectors
    actionsLog.clear();
    volunteers.clear();
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    customers.clear();


    // copying vectors:
    for (BaseAction *act : other.actionsLog)
    {
        this->actionsLog.push_back(act->clone());
    }
    for (Volunteer *vol : other.volunteers)
    {
        this->volunteers.push_back(vol->clone());
    }
    for (Order *order : other.pendingOrders)
    {
        this->pendingOrders.push_back(order->clone());
    }
    for (Order *order : other.inProcessOrders)
    {
        this->inProcessOrders.push_back(order->clone());
    }
    for (Order *order : other.completedOrders)
    {
        this->completedOrders.push_back(order->clone());
    }
    for (Customer *cust : other.customers)
    {
        this->customers.push_back(cust->clone());
    }

    // copying defaults
    this->defaultCustomer = other.defaultCustomer->clone();
    this->defaultVolunteer = other.defaultVolunteer->clone();
    this->defaultOrder = other.defaultOrder->clone();

    return *this;
}

WareHouse& WareHouse::operator=(WareHouse &&other) {
    if (this == &other){
        return *this;
    }

    this->isOpen = other.isOpen;
    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;
    this->orderCounter = other.orderCounter;
    this->actionsLog = std::move(other.actionsLog);
    this->volunteers = std::move(other.volunteers);
    this->pendingOrders = std::move(other.pendingOrders);
    this->inProcessOrders = std::move(other.inProcessOrders);
    this->completedOrders = std::move(other.completedOrders);
    this->customers = std::move(other.customers);

    // copying defaults
    this->defaultCustomer = other.defaultCustomer;
    this->defaultOrder = other.defaultOrder;
    this->defaultVolunteer = other.defaultVolunteer;

    // nulling other's defaults
    other.defaultCustomer = nullptr;
    other.defaultOrder = nullptr;
    other.defaultVolunteer = nullptr;

    return *this;
}