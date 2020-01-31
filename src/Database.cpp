#include "Database.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

bool Database::init(std::vector<std::string> trains, std::vector<Order> orders)
{
    if(trains.empty())
        return false;
    for(string s:trains)
    {
        ifstream f(s);
        if(!f.good())
            return false;
        string name;
        int size;
        f >> name >> size;
        _trainSizes[name]=size;
        vector<pair<string,int>> scedule;
        string station;
        int time;
        while(f >> station >> time)
        {
            if(time==1)
                scedule.push_back(pair<string,int>(station,0));
            scedule.push_back(pair<string,int>(station,time));
        }
        _scedules[name]=scedule;
        _periods[name]=scedule.back().second;
    }
    for(Order& o:orders)
    {
        map<string,int> products=_destination_state[o._destination];
        products[o._name]+=o._quantity;
        _destination_state[o._destination]=products;
    }
    _orders=orders;
    return true;
}

bool Database::hasOrdersArrived(map<string,map<string,int>> stations)
{
    for(Order o:_orders)
    {
        if(!o.hasArrived(stations))
            return false;
    }
    return true;
}

string Database::whereIsThisTrain(std::string name, int time)
{
    vector<pair<string,int>> scedule=_scedules[name];
    int t=time;
    int period=_periods[name];
    while(t-period>=0)
    {
//        auto it=find_if(scedule.begin(),scedule.end(),[time](const pair<string,int>& p)
//        {
//            return p.second==time;
//        });
//        if(it!=scedule.end())
//            return it->first;
        for(auto p:scedule)
        {
            if(p.second==t)
                return p.first;
        }
        t=t-period;
    }
    return "";
}

std::vector<std::string> Database::getStationTrains(std::string station_name, int time)
{
    vector<std::string> res;
    for(auto train:_trainSizes)
    {
        if(whereIsThisTrain(train.first,time)==station_name)
            res.push_back(train.first);
        //cout<<whereIsThisTrain(train.first,time)<< " " << station_name<< " " << time<<endl;
    }
    return res;
}
