#include "Database.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

bool Database::init(std::vector<std::string> trains)
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
    }
    return true;
}

bool Database::hasOrdersArrived(map<string,map<string,int>> stations)
{
    for(Order& o:_orders)
    {
        if(!o.hasArrived(stations))
            return false;
    }
    return true;
}

string Database::whereIsThisTrain(string name, int time)
{
    vector<pair<string,int>> scedule=_scedules[name];
    auto it=find_if(scedule.begin(),scedule.end(),[time](const pair<string,int>& p)
    {
        return p.second==time;
    });
    if(it!=scedule.end())
        return it->first;
    it=max_element(scedule.begin(),scedule.end(),[](const pair<string,int>& p1,const pair<string,int>& p2)
    {
        return p1.second<p2.second;
    });
    if(time>it->second)
    {
        for(auto a:scedule)
            scedule.push_back(pair<string,int>(a.first,a.second+it->second));
        whereIsThisTrain(name,time);
    }
    return "";
}
