#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <map>
#include <set>
#include "Order.h"

class Database
{
    public:
        Database(){}
        bool init(std::vector<std::string> trains,std::vector<Order> orders);
        bool hasOrdersArrived(std::map<std::string,std::map<std::string,int>> stations);
        std::string whereIsThisTrain(std::string name, int time);
        std::vector<std::string> getStationTrains(std::string station_name, int time);


        std::map<std::string,std::vector<std::pair<std::string,int>>> GetScedules() { return _scedules; }
        std::map<std::string,int> GetTrainSizes() { return _trainSizes; }
        std::vector<Order> GetOrders() { return _orders; }
        std::map<std::string,std::map<std::string,int>> GetDestinationState() { return _destination_state; }

    protected:

    private:
        std::map<std::string,std::vector<std::pair<std::string,int>>> _scedules;
        std::map<std::string,int> _trainSizes;
        std::vector<Order> _orders;
        std::map<std::string,std::map<std::string,int>> _destination_state;
        std::map<std::string,int> _periods;
};

#endif // DATABASE_H
