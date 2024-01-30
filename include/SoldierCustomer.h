#pragma once
#include <string>
#include <vector>
#include "../include/Customer.h"
using std::string;
using std::vector;

class SoldierCustomer : public Customer
{
public:
    SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders);
    SoldierCustomer *clone() const override;

    ~SoldierCustomer();

private:
};