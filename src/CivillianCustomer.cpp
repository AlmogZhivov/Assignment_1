#include "Customer.h"
#include "CivillianCustomer.h"

class CivillianCustomer : public Customer {

    CivillianCustomer::CivillianCustomer(int id, const string &name, int locationDistance, int maxOrders) :
        Customer(id, name, locationDistance, maxOrders) {}

    
    CivilianCustomer* clone() const {
        return new CivillianCustomer(*this);
    }

    CivillianCustomer::~CivillianCustomer() {}
}