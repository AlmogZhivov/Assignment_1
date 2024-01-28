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
	vector<Volunteer*> volunteers = wareHouse.getVolunteers();
	for (int i = 1; i <= numOfSteps; i++) {
		for (Order* pendingOrder: pendingOrders) {
			for (Volunteer *volunteer: wareHouse.getVolunteers()) {
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
		for (Volunteer* volunteer: wareHouse.getVolunteers()) {
			volunteer->step();
		}
		for (Volunteer* volunteer: wareHouse.getVolunteers()) {
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
		for (Volunteer* volunteer: wareHouse.getVolunteers()) {
			if (!volunteer->hasOrdersLeft()) {
				delete volunteer;
				volunteers.erase(remove_if(volunteers.begin(), volunteers.end(),
											[volunteer](const Volunteer* o) { return o == volunteer; }),
								volunteers.end());
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
	if (!wareHouse.orderExists(orderId))
		error("Order doesn't exist");
	Order &order = wareHouse.getOrder(orderId);
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
	if (!wareHouse.customerExists(customerId))
		error("Customer doesn't exist");
	Customer &customer = wareHouse.getCustomer(customerId);
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
	if (!wareHouse.volunteerExists(volunteerId))
		error("Volunteer doesn't exist");
	Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);
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
