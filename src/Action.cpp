#include "../include/Action.h"
extern WareHouse *backup;
#include <algorithm>

// BaseAction Class
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED)
{
}

ActionStatus BaseAction::getStatus() const
{
	return status;
}

void BaseAction::setActionString(std::string str)
{
	actionString = str;
}

void BaseAction::complete()
{
	status = ActionStatus::COMPLETED;
}

void BaseAction::error(std::string errorMsg)
{
	status = ActionStatus::ERROR;
	this->errorMsg = errorMsg;
}
std::string BaseAction::getErrorMsg() const
{
	return errorMsg;
}

// SimulateStep Class
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) 
{

}
void SimulateStep::act(WareHouse &wareHouse)
{
	vector<Order*> pendingOrders = wareHouse.getPendingOrders();
	vector<Order*> inProcessOrders = wareHouse.getInProcessOrders();
	vector<Order*> completedOrders = wareHouse.getCompletedOrders();
	for (int i = 1; i <= numOfSteps; i++) {
		for (Order* pendingOrder: pendingOrders) {
			for (Volunteer *volunteer: wareHouse.getVolunteers()) {
				if(pendingOrder->getStatus() == OrderStatus::PENDING && volunteer->canTakeOrder(*pendingOrder)) {
					// Collector
					if (typeid(volunteer) == typeid(CollectorVolunteer)) {
						CollectorVolunteer *v = dynamic_cast<CollectorVolunteer*>(volunteer);
						v->step();
						pendingOrders.erase(remove_if(pendingOrders.begin(), pendingOrders.end(),
											[pendingOrder](const Order* o) { return o == pendingOrder; }),
								pendingOrders.end());
						pendingOrder->setCollectorId(v->getId());
						pendingOrder->setStatus(OrderStatus::COLLECTING);
						inProcessOrders.push_back(pendingOrder);
					}
					// Limited Collector
					else if (typeid(volunteer) == typeid(LimitedCollectorVolunteer)) {
						LimitedCollectorVolunteer *v = dynamic_cast<LimitedCollectorVolunteer*>(volunteer);
						v->step();
						pendingOrders.erase(remove_if(pendingOrders.begin(), pendingOrders.end(),
											[pendingOrder](const Order* o) { return o == pendingOrder; }),
								pendingOrders.end());
						pendingOrder->setCollectorId(v->getId());
						pendingOrder->setStatus(OrderStatus::COLLECTING);
						inProcessOrders.push_back(pendingOrder);
					}
					// Driver
					else if (typeid(volunteer) == typeid(DriverVolunteer)) {
						DriverVolunteer *v = dynamic_cast<DriverVolunteer*>(volunteer);
						v->step();
						pendingOrders.erase(remove_if(pendingOrders.begin(), pendingOrders.end(),
											[pendingOrder](const Order* o) { return o == pendingOrder; }),
								pendingOrders.end());
						pendingOrder->setCollectorId(v->getId());
						pendingOrder->setStatus(OrderStatus::DELIVERING);
						inProcessOrders.push_back(pendingOrder);
					}
					// Limited Driver
					else if (typeid(volunteer) == typeid(LimitedDriverVolunteer)) {
						LimitedDriverVolunteer *v = dynamic_cast<LimitedDriverVolunteer*>(volunteer);
						v->step();
						pendingOrders.erase(remove_if(pendingOrders.begin(), pendingOrders.end(),
											[pendingOrder](const Order* o) { return o == pendingOrder; }),
								pendingOrders.end());
						pendingOrder->setCollectorId(v->getId());
						pendingOrder->setStatus(OrderStatus::DELIVERING);
						inProcessOrders.push_back(pendingOrder);
					}
				}
			}
		}
		for (Volunteer* volunteer: wareHouse.getVolunteers()) {
			if (!volunteer->hasOrdersLeft()) {
				if (typeid(volunteer) == typeid(LimitedCollectorVolunteer)) {
					LimitedCollectorVolunteer *v = dynamic_cast<LimitedCollectorVolunteer*>(volunteer);
					if (v->getNumOrdersLeft() == 0)
						delete v;
				}
				else if (typeid(volunteer) == typeid(LimitedDriverVolunteer)) {
					LimitedDriverVolunteer *v = dynamic_cast<LimitedDriverVolunteer*>(volunteer);
					if (v->getNumOrdersLeft() == 0)
						delete v;
				}
				else if (typeid(volunteer) == typeid(CollectorVolunteer)) {
					CollectorVolunteer *v = dynamic_cast<CollectorVolunteer*>(volunteer);
					Order &order = wareHouse.getOrder(v->getActiveOrderId());
					order.setStatus(OrderStatus::COLLECTING);
					pendingOrders.push_back(&order);
				}
				else if (typeid(volunteer) == typeid(DriverVolunteer)) {
					DriverVolunteer *v = dynamic_cast<DriverVolunteer*>(volunteer);
					Order &order = wareHouse.getOrder(v->getActiveOrderId());
					order.setStatus(OrderStatus::COMPLETED);
					completedOrders.push_back(&order);
				}
			}
		}
	}
}
string SimulateStep::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg();
}
SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}
// AddOrder Class
AddOrder::AddOrder(int id) : customerId(id) 
{
}
void AddOrder::act(WareHouse &wareHouse)
{
	Customer &customer = wareHouse.getCustomer(customerId);
	if (customer.getId() == -1 || !customer.canMakeOrder())
		cout << "Cannot place this order”" << endl;
	Order *order = new Order(wareHouse.getOrderCounter(), customerId, customer.getCustomerDistance());
	order->setStatus(OrderStatus::PENDING);
	customer.addOrder(order->getId());
	wareHouse.addOrder(order);
	complete();
	wareHouse.addAction(new AddOrder(*this));
}
string AddOrder::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg();
}
AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}
// AddCustomer Class
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : customerName(customerName), customerType((customerType == "Soldier")? CustomerType::Soldier:CustomerType::Civilian), distance(distance), maxOrders(maxOrders) 
{
}
void AddCustomer::act(WareHouse &wareHouse)
{
	if (EnumToStringCustomer(CustomerType::Soldier) == "Soldier")
		wareHouse.addCustomer(new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders));
	else if (EnumToStringCustomer(CustomerType::Civilian) == "Civilian")
		wareHouse.addCustomer(new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders));
	complete();
	wareHouse.addAction(new AddCustomer(*this));
}
string AddCustomer::EnumToStringCustomer(CustomerType type)
{
    switch (type)
    {
        case CustomerType::Soldier:   
            return "Soldier";
        case CustomerType::Civilian:   
            return "Civilian";
    }
    return "";
}
string AddCustomer::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg();
}
AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}
// PrintOrderStatus Class
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id)
{
}
void PrintOrderStatus::act(WareHouse &wareHouse)
{
	Order &order = wareHouse.getOrder(orderId);
	if (order.getId() == -1)
		error("Order doesn't exist");
	cout << order.toString() << endl;
	complete();
	wareHouse.addAction(new PrintOrderStatus(*this));
}
string PrintOrderStatus::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg();
}
PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

// PrintCustomerStatus Class
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId)
{
}
void PrintCustomerStatus::act(WareHouse &wareHouse)
{
	Customer &customer = wareHouse.getCustomer(customerId);
	if (customer.getId() == -1)
		error("Customer doesn't exist");
	cout << "CustomerId: " + customerId << endl;
	for (int orderId: customer.getOrdersIds())
	{
		cout << "OrderId: " + orderId << endl;
		Order &order =  wareHouse.getOrder(orderId);
		cout << "OrderStatus: " + order.EnumToStringOrder(order.getStatus()) << endl;
	}
	cout << "NumOrdersLeft: " + customer.getMaxOrders() - customer.getNumOrders() << endl;
	complete();
	wareHouse.addAction(new PrintCustomerStatus(*this));
}
string PrintCustomerStatus::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg();
}
PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}
// PrintVolunteerStatus Class
PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) 
{
}
void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
	Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);
	if (volunteer.getId() == -1)
		error("Volunteer doesn't exist");
	cout << volunteer.toString() << endl;
	complete();
	wareHouse.addAction(new PrintVolunteerStatus(*this));
}
string PrintVolunteerStatus::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg();
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}
// PrintActionsLog Class
PrintActionsLog::PrintActionsLog() 
{
}
void PrintActionsLog::act(WareHouse &wareHouse)
{
    vector<BaseAction *>::const_iterator it;
	for (it = wareHouse.getActions().begin(); it != wareHouse.getActions().end(); it++)
	{
		cout << (*it)->toString() << endl;
	}
	complete();
	wareHouse.addAction(new PrintActionsLog(*this));
}
string PrintActionsLog::toString() const
{
    return actionString + " Completed";
}
PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
// Close Class - free memory?
Close::Close()
{
}
void Close::act(WareHouse &wareHouse)
{
	for (Order* order : wareHouse.getPendingOrders()) {
		cout << "OrderId: " + order->getId() << " ";
		cout << "CustomerId: " + order->getCustomerId() << " ";
		cout << "OrderStatus: " + order->EnumToStringOrder(order->getStatus());
	}
    for (Order* order : wareHouse.getInProcessOrders()) {
		cout << "OrderId: " + order->getId() << " ";
		cout << "CustomerId: " + order->getCustomerId() << " ";
		cout << "OrderStatus: " + order->EnumToStringOrder(order->getStatus());
	}
	for (Order* order : wareHouse.getCompletedOrders()) {
		cout << "OrderId: " + order->getId() << " ";
		cout << "CustomerId: " + order->getCustomerId() << " ";
		cout << "OrderStatus: " + order->EnumToStringOrder(order->getStatus());
	}
	wareHouse.close();
	complete();
	wareHouse.addAction(new Close(*this));
}
string Close::toString() const
{
    return string();
}
Close *Close::clone() const
{
    return new Close(*this);
}
// BackupWareHouse Class
BackupWareHouse::BackupWareHouse()
{
}
void BackupWareHouse::act(WareHouse &wareHouse)
{
    if (backup != nullptr)
		delete backup;
	complete();
	wareHouse.addAction(new BackupWareHouse(*this));
	backup = new WareHouse(wareHouse);
}
string BackupWareHouse::toString() const
{
    return actionString + " Completed";
}
BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}
// RestoreWareHouse Class
RestoreWareHouse::RestoreWareHouse()
{
}
void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup == nullptr)
	{ 
		error("No backup available");
		cout << "No backup available" << endl;
	}
	else
	{
		wareHouse = *backup;
		complete();
	}
	wareHouse.addAction(new RestoreWareHouse(*this));
}
string RestoreWareHouse::toString() const
{
    if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg();
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
