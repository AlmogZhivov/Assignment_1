#pragma once
#include <string>
#include <vector>
#include "Customer.h"
using std::string;
using std::vector;

class CivilianCustomer : public Customer
{
public:
    CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders);
    CivilianCustomer *clone() const override;

    ~CivilianCustomer() override;

private:
};