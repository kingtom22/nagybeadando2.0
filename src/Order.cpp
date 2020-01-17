#include "Order.h"
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

Order::Order(std::string name,std::string source, std::string destination, int quantity) :
    _name(name), _source(source), _destination(destination), _quantity(quantity)
{
    //ctor
}

bool Order::hasArrived(map<string,map<string,int>>& stations)
{
    for(auto& station: stations)
    {
        auto it=station.second.find(_name);
        if(it!=station.second.end())
        {
            if(station.first==_destination && it->second==_quantity)
                return true;
        }
    }
    return false;
}
