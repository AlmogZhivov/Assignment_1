#include "../include/Customer.h"
#include <algorithm>
#include <string>
using namespace std;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders)
{
    
}

SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders)
{
}
SoldierCustomer *SoldierCustomer::clone() const
{
    return new SoldierCustomer(*this);
}
CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders)
{
}
CivilianCustomer *CivilianCustomer::clone() const
{
    return new CivilianCustomer(*this);
}