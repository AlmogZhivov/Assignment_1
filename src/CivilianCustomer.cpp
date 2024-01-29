#include "Customer.h"
#include "../include/CivilianCustomer.h"


    CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) :
        Customer(id, name, locationDistance, maxOrders) {}

    
    CivilianCustomer* CivilianCustomer::clone() const {
        return new CivilianCustomer(*this);
    }

    CivilianCustomer::~CivilianCustomer() {}
