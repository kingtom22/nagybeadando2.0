#include <iostream>
#include <algorithm>
#include "ActionPlan.h"

using namespace std;

ActionPlan::ActionPlan(std::vector<Train>& trains, std::vector<Order>& orders, std::vector<Wagon>& wagons) :
    _trains(trains), _orders(orders), _wagons(wagons)
{
    //ctor
    _time=0;
    for(Order& o:orders)
    {
        map<string,int> products;
        if(_stations.find(o._source)==_stations.end())
            products[o._name]=o._quantity;
        else
        {
            products=_stations.find(o._source)->second;
            if(products.find(o._name)==products.end())
                products[o._name]=o._quantity;
            else
                products[o._name]=o._quantity;
        }
        _stations[o._source]=products;
    }
}

void ActionPlan::canProductsBeDelivered()
{
    for(vector<Order>::iterator it=_orders.begin();it!=_orders.end();it++)
    {
        if(!it->canReachDestination(_trains))
        {
            cout<<"Hiba! "<<it->_name<<" can't reach it's destination."<<endl;
            _orders.erase(it);
            it--;
        }
    }
}

void ActionPlan::CreatePlan()
{
    while(_orders.size()>0)
    {
        /// To Do ///
        // do possible pakolás
        for(auto& station:_stations)
        {
            string place=station.first;
            map<string,int> products=station.second;
            vector<Wagon> station_wagons;
            copy_if(_wagons.begin(),_wagons.end(),back_inserter(station_wagons),
                    [place](const Wagon& w){return w._place==place;});
            for(Wagon& w:station_wagons)
            {
                /// felpakolás
                // ha nem érkezett meg az áru és megy arra vonat ami a célállomása felé megy akkor pakolja fel
                for(auto& p:products)
                {
                    string name=p.first;
                    auto it=find_if(_orders.begin(),_orders.end(),
                                    [name](const Order& o)
                    {
                        return o._name==name;
                    });
                    if(it!=_orders.end() && !it->hasArrived(_stations) && w._load<w._size)
                    {
                        //bármi van felpakolom mert nem tudom leírni a felpakolás szempontjait
                        w.pakol(name,products);
                        cout << _time << " pakol " << place << " " << w._name << " " << name << endl;
                    }
                }
                /// lepakolás
            }
            station.second=products;
        }

        // do possible csatlakoztatás

        // move trains
#ifdef DEBUG
        writeStateToConsole();
#endif // DEBUG

        _time++;
        return;
    }
}

void ActionPlan::writeStateToConsole()
{
    for(Train& T:_trains)
    {
        cout << _time << " " << T._name << " " << T._place << " csatolt vagonok:"<<endl;
        for(Wagon* W:T._wagons)
            cout <<"-"<<W->_name<<" "<<W->_load<<" "<<W->_product<<endl;
        cout<<endl;
    }
    cout<<"Vagonok:"<<endl;
    for(Wagon& w:_wagons)
        cout<<"-"<<w._name<<" "<<w._place<<" "<<w._product<<" "<<w._load<<endl;
    cout<<endl<<"Termékek: "<<endl;
    for(pair<string,map<string,int>> station:_stations)
    {
        cout<<station.first<<":"<<endl;
        for(pair<string,int> product:station.second)
            cout<<"-"<<product.first<<" "<<product.second<<endl;
        cout<<endl;
    }
}
