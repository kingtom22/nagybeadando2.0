#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <map>
#include "Order.h"
#include "Wagon.h"

class Database
{
    public:
        Database(){}
        bool init(std::vector<std::string> trains);
        bool hasOrdersArrived(std::map<std::string,std::map<std::string,int>> stations);
        std::string whereIsThisTrain(std::string name, int time);

        std::map<std::string,std::vector<std::pair<std::string,int>>> GetScedules() { return _scedules; }
        std::map<std::string,int> GetTrainSizes() { return _trainSizes; }
        std::vector<Order> GetOrders() { return _orders; }
        void SetOrders(std::vector<Order> val) { _orders = val; }

    protected:

    private:
        std::map<std::string,std::vector<std::pair<std::string,int>>> _scedules;
        std::map<std::string,int> _trainSizes;
        std::vector<Order> _orders;
};

#endif // DATABASE_H
