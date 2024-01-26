#include "../include/Order.h"
using namespace std;
//dummy
const string Order::toString() const
{
    return "";
}
string Order::EnumToString(OrderStatus type)
{
    switch (type)
    {
        case OrderStatus::PENDING:   
            return "Pending";
        case OrderStatus::COLLECTING:   
            return "Collecting";
        case OrderStatus::DELIVERING: 
            return "Delivering";
        case OrderStatus::COMPLETED: 
            return "Completed";
    }
    return "";
}