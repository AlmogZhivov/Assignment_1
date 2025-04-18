#include "../include/Action.h"
extern WareHouse *backup;
#include <algorithm>
#include "../include/CivilianCustomer.h"
#include "../include/SoldierCustomer.h"
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
BaseAction::~BaseAction()
{
}
// SimulateStep Class
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) 
{

}
void SimulateStep::act(WareHouse &wareHouse)
{
	wareHouse.simulateStep(numOfSteps);
	this->actionString = "simulateStep " + to_string(numOfSteps);
	wareHouse.addAction(new SimulateStep(*this));
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
	if (customer.getId() == -1 || !customer.canMakeOrder()) {
		cout << "Error: Cannot place this order" << endl;
		error("");
		wareHouse.addAction(new AddOrder(*this));
	}
	else {
		Order *order = new Order(wareHouse.getOrderCounter(), customerId, customer.getCustomerDistance());
		order->setStatus(OrderStatus::PENDING);
		customer.addOrder(order->getId());
		wareHouse.addOrder(order);
		complete();
		wareHouse.addAction(new AddOrder(*this));
	}
}
string AddOrder::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error" + getErrorMsg();
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
	if (!wareHouse.orderExists(orderId)) {
		cout << "Error: Order doesn't exist" << endl;
		error("");
		wareHouse.addAction(new PrintOrderStatus(*this));
	}
	else {
		Order &order = wareHouse.getOrder(orderId);
		cout << order.toString() << endl;
		complete();
		wareHouse.addAction(new PrintOrderStatus(*this));
	}
}
string PrintOrderStatus::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error" + getErrorMsg();
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
	if (!wareHouse.customerExists(customerId)) {
		cout << "Error: Customer doesn't exist" << endl;
		error("");
		wareHouse.addAction(new PrintCustomerStatus(*this));
	}
	else {
		Customer &customer = wareHouse.getCustomer(customerId);
		cout << "CustomerId: " + to_string(customerId) << endl;
		for (int orderId: customer.getOrdersIds())
		{
			cout << "OrderId: " + to_string(orderId) << endl;
			Order &order =  wareHouse.getOrder(orderId);
			cout << "OrderStatus: " + order.getStringStatus() << endl;
		}
		cout << "NumOrdersLeft: " + to_string(customer.getMaxOrders() - customer.getNumOrders()) << endl;
		complete();
		wareHouse.addAction(new PrintCustomerStatus(*this));
	}	
}
string PrintCustomerStatus::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error" + getErrorMsg();
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
	if (volunteer.getId() == NO_VOLUNTEER)
	{
		cout << "Error: Volunteer doesn't exist" << endl;
		error("");
		wareHouse.addAction(new PrintVolunteerStatus(*this));
	}
	else {
		cout << volunteer.toString() << endl;
		complete();
		wareHouse.addAction(new PrintVolunteerStatus(*this));
	}
}
string PrintVolunteerStatus::toString() const
{
	if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error" + getErrorMsg();
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
// Close Class
Close::Close()
{
}
void Close::act(WareHouse &wareHouse)
{
	cout << wareHouse.stringOrdersWhenClose() << endl;
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
	//change order
	backup = new WareHouse(wareHouse);
	wareHouse.addAction(new BackupWareHouse(*this));
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
		error("");
		cout << "Error: No backup available" << endl;
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
		return actionString + " Error" + getErrorMsg();
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
