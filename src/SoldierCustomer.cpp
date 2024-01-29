#include "Customer.h"
#include "SoldierCustomer.h"

class SoldierCustomer {

    SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) :
        Customer(id, name, locationDistance, maxOrders) {}

    SoldierCustomer* SoldierCustomer::clone() const {
        return new SoldierCustomer(*this);
    }

    SoldierCustomer::~SoldierCustomer() {}
}