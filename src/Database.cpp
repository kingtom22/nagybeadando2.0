#include "Database.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//lkkt seged
int fv(int a, int b)
{
    if(a==0)
        return b;
    return fv(b%a, a);
}
//lkkt
int lkkt(set<int>& s)
{
    int a, b;
    while(s.size()>1)
    {
        a=*s.begin();
        s.erase(s.begin());
        b=*s.begin();
        s.erase(s.begin());
        s.insert(a*b/fv(a,b));
    }
    return *s.begin();
}

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
    while(time-_periods[name]>0)
    {
        vector<pair<string,int>> scedule=_scedules[name];
        auto it=find_if(scedule.begin(),scedule.end(),[time](const pair<string,int>& p)
        {
            return p.second==time;
        });
        if(it!=scedule.end())
            return it->first;
        time-=_periods[name];
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
    }
    return res;
}
