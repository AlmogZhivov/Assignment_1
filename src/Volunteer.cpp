#include "../include/Volunteer.h"
#include <iostream>

using namespace std;
Volunteer::Volunteer(int id, const string &name) : id(id), name(name)
{
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown)
{
}
CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}
string CollectorVolunteer::toString() const
{
}
bool CollectorVolunteer::hasOrdersLeft() const
{
}
bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
}
void CollectorVolunteer::acceptOrder(const Order &order)
{
}
void CollectorVolunteer::step() 
{
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders)
{
}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}
string LimitedCollectorVolunteer::toString() const
{
}
bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep)
{
}
DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer(*this);
}
string DriverVolunteer::toString() const
{
}
bool DriverVolunteer::hasOrdersLeft() const
{
}
bool DriverVolunteer::canTakeOrder(const Order &order) const
{
}
void DriverVolunteer::acceptOrder(const Order &order)
{
}
void DriverVolunteer::step() 
{
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders)
{
}
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}
string LimitedDriverVolunteer::toString() const
{
}
bool LimitedDriverVolunteer::hasOrdersLeft() const
{
}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
}
void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
}