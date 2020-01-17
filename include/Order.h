#ifndef ORDER_H
#define ORDER_H

#include "Train.h"
#include <map>

class Order
{
    public:
        Order(std::string name,std::string source, std::string destination, int quantity);
        bool hasArrived(std::map<std::string,std::map<std::string,int>>& _stations);

        std::string _name;
        std::string _source;
        std::string _destination;
        int _quantity;

    protected:

    private:
};

#endif // ORDER_H
