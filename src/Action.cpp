#include "../include/Action.h"
extern WareHouse *backup;

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
}
string SimulateStep::toString() const
{
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
}
string AddOrder::toString() const
{
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
}
string AddCustomer::toString() const
{
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
	if (&order == nullptr)
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
	if (&customer == nullptr)
		error("Customer doesn't exist");
	cout << "CustomerId: " + customerId << endl;
	for (int orderId: customer.getOrdersIds())
	{
		cout << "OrderId: " + orderId << endl;
		Order &order =  wareHouse.getOrder(orderId);
		cout << "OrderStatus: " + order.EnumToString(order.getStatus()) << endl;
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
// PrintVolunteerStatus Class - TODO Simplification
PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) 
{
}
void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
	Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);
	if (&volunteer == nullptr)
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
// Close Class - TODO
Close::Close()
{
}
void Close::act(WareHouse &wareHouse)
{
	for (Order* order : wareHouse.getPendingOrders()) {
		cout << "OrderId: " + order->getId() << " ";
		cout << "CustomerId: " + order->getCustomerId() << " ";
		cout << "OrderStatus: " + order->EnumToString(order->getStatus());
	}
    for (Order* order : wareHouse.getInProcessOrders()) {
		cout << "OrderId: " + order->getId() << " ";
		cout << "CustomerId: " + order->getCustomerId() << " ";
		cout << "OrderStatus: " + order->EnumToString(order->getStatus());
	}
	for (Order* order : wareHouse.getCompletedOrders()) {
		cout << "OrderId: " + order->getId() << " ";
		cout << "CustomerId: " + order->getCustomerId() << " ";
		cout << "OrderStatus: " + order->EnumToString(order->getStatus());
	}
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
