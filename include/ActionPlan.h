#ifndef ACTIONPLAN_H
#define ACTIONPLAN_H

#include "Order.h"

class ActionPlan
{
    public:
        ActionPlan(std::vector<Train>& trains, std::vector<Order>& orders, std::vector<Wagon>& wagons);
        bool areProductsDelivered();
        void canProductsBeDelivered();
        void CreatePlan();
        void writeStateToConsole();

        std::vector<Train> _trains;
        std::vector<Order> _orders;
        std::vector<Wagon> _wagons;
        std::map<std::string,std::map<std::string,int>> _stations;// hol miből mennyi van lepakolva
        int _time;
    protected:

    private:
};

#endif // ACTIONPLAN_H
