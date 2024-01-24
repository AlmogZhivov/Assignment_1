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
}
string PrintOrderStatus::toString() const
{
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
}
string PrintCustomerStatus::toString() const
{
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
}
string PrintVolunteerStatus::toString() const
{
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
}
string PrintActionsLog::toString() const
{
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
}
string Close::toString() const
{
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
}
string BackupWareHouse::toString() const
{
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
}
string RestoreWareHouse::toString() const
{
    if (getStatus() == ActionStatus::COMPLETED)
		return actionString + " Completed";
	else
		return actionString + " Error: " + getErrorMsg()
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}
