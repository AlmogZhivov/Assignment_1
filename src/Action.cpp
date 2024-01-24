#include "../include/Action.h"
#include "../include/WareHouse.h"
extern WareHouse *backup;

// BaseAction Class
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED)
{
}

BaseAction::BaseAction(const BaseAction &other) : actionString(other.actionString), errorMsg(other.errorMsg), status(other.status)
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
}
SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}
// AddOrder Class
// AddCustomer Class
// PrintOrderStatus Class
// PrintCustomerStatus Class
// PrintVolunteerStatus Class
// PrintActionsLog Class
// Close Class
// BackupWareHouse Class
// RestoreWareHouse Class
