#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>

#include "Database.h"
#include "Wagon.h"

using namespace std;

struct A
{
    map<string,map<string,int>> stations;//melyik állomáson mibõl mennyi van
    vector<Wagon> wagons;
    vector<A> history;
    A(vector<Wagon> start_wagons,vector<Order> orders)
    {
        wagons=start_wagons;
        for(Order& o:orders)
        {
            map<string,int> products=stations[o._source];
            products[o._name]+=o._quantity;
            stations[o._source]=products;
        }
        /*for(auto station:stations)
        {
            cout<<station.first<<endl;
            for(auto p:station.second)
                cout<< p.first <<" "<<p.second<<endl;
        }*/
    }
    vector<A> gyerekek(Database& DB)
    {
        vector<A> res;
        if(history.empty()) // t=0
        {
            for(size_t i=0; i<wagons.size(); i++)
            {
                map<string,int> products=stations[wagons[i].getPlace()];
                for(pair<string,int> p:products)
                {
                    A uj(*this);
                    uj.history.push_back(uj);
                    map<string,int> new_products=uj.stations[wagons[i].getPlace()];
                    int q=new_products[p.first];
                    if(q<=wagons[i].getSize())
                    {
                        uj.wagons[i].pakol(p.first,q);
                        new_products.erase(p.first);
                        uj.stations[wagons[i].getPlace()]=new_products;
                        res.push_back(uj);
                    }
                    else
                    {
                        new_products[p.first]-=wagons[i].getSize()-wagons[i].load();
                        uj.stations[wagons[i].getPlace()]=new_products;
                        res.push_back(uj);
                    }
                }
            }

        }
        else // t!=0
        {
            A semmi(*this);
            semmi.history.push_back(semmi);
            res.push_back(semmi);
            A uj(*this);
            uj.history.push_back(uj);
            for(size_t i=0; i<wagons.size(); i++)
            {
                // pakolás
                if(!wagons[i].isOnTrain())
                {
                    map<string,int> products=stations[wagons[i].getPlace()];
                    for(pair<string,int> p:products)
                    {
                        A uj(*this);
                        uj.history.push_back(uj);
                        map<string,int> new_products=uj.stations[wagons[i].getPlace()];
                        int q=new_products[p.first];
                        if(q<=wagons[i].getSize()-wagons[i].load())
                        {
                            uj.wagons[i].pakol(p.first,q);
                            new_products.erase(p.first);
                            uj.stations[wagons[i].getPlace()]=new_products;
                            res.push_back(uj);
                        }
                        else
                        {
                            uj.wagons[i].pakol(p.first,wagons[i].getSize()-wagons[i].load());
                            new_products[p.first]-=wagons[i].getSize()-wagons[i].load();
                            uj.stations[wagons[i].getPlace()]=new_products;
                            res.push_back(uj);
                        }
                    }
                }

                // lepakolás
                if(!wagons[i].isOnTrain())
                {
                    vector<Order> orders=DB.GetOrders();
                    string place=wagons[i].getPlace();
                    auto it=find_if(orders.begin(),orders.end(),[place](const Order& o)
                    {
                        return o._destination==place;
                    });
                    if(it!=orders.end())
                    {
//                        A uj(*this);
//                        uj.history.push_back(uj);
                        map<string,int> station_products=stations[place];
                        map<string,int> wagon_products=wagons[i].getProducts();
                        uj.wagons[i].lepakol(it->_name,wagon_products[it->_name]);
                        res.push_back(uj);
                    }
                }

                // csatolás
                if(!wagons[i].isOnTrain())
                {
                    vector<string> station_trains=DB.getStationTrains(wagons[i].getPlace(),history.size());
                    for(string train:station_trains)
                    {
//                        A uj(*this);
//                        uj.history.push_back(uj);
                        uj.wagons[i].csatol(train);
                        res.push_back(uj);
                    }
                }

                // lecsatolás
                if(wagons[i].isOnTrain())
                {
                    string place=DB.whereIsThisTrain(wagons[i].getTrain(),history.size());
                    if(!place.empty())
                    {
//                        A uj(*this);
//                        uj.history.push_back(uj);
                        uj.wagons[i].lecsatol(place);
                        res.push_back(uj);
                    }
                }
            }
        }
        return res;
    }
};
float value(const A& a,Database DB)
{
    map<string,map<string,int>> destination_stations=DB.GetDestinationState();
    map<string,map<string,int>> actual_stations=a.stations;
    int res=10;
    for(auto station:destination_stations)
    {
        for(pair<string,int> p:station.second)
        {
            map<string,int> actual_products=actual_stations[station.first];
            res+=actual_products[p.first];
        }
    }
    return (res/1000); //TODO
}

bool operator<(const A& a, const A& b)
{
    if(a.stations!=b.stations)
        return a.stations<b.stations;
    for(size_t i=0; i<a.wagons.size(); i++)
    {
        if(a.wagons[i]!=b.wagons[i])
            return a.wagons[i]<b.wagons[i];
    }
    return a.history.size()<b.history.size();
}

bool vegeredmeny(const A& a,Database DB)
{
    return DB.hasOrdersArrived(a.stations);
}
ostream & operator<<(ostream &out, const A& a)
{
    for(size_t t=0; t<a.history.size(); t++)
    {
        cout << t << " idõpontban "<<endl;
        for(auto& station:a.history[t].stations)
        {
            cout<< "--------" << station.first << "--------"<< endl;

            cout<< "lepakolva:" << endl;
            for(auto product:station.second)
                cout << "\t" << product.first << "\t" << product.second <<endl;
        }
        for(Wagon w:a.history[t].wagons)
        {
            cout << w.getName() << endl;
            if(w.isOnTrain())
                cout<< ": vonaton: " << w.getTrain() <<endl;
            for(auto product:w.getProducts())
                cout << "\t" << product.first << "\t" << product.second <<endl;
        }
    }
    cout<<"*****************************"<<endl;

    return out;
}
template<typename T>
class NodeSet
{
    set<T> t;
    Database DB;
public:
    NodeSet(Database db)
    {
        DB=db;
    }
    T pop_max()
    {
        auto it_max = t.begin();
        for (auto it=t.begin(); it!=t.end(); it++)
        {
            if (value(*it,DB) > value(*it_max,DB))
            {
                it_max = it;
            }
        }
        T res = *it_max;
        t.erase(it_max);
        return res;
    }
    void put(T a)
    {
        t.insert(a);
    }
    int size() const
    {
        return t.size();
    }
    void dump(ostream &out = cout)
    {
        for (auto it=t.begin(); it!=t.end(); it++)
        {
            out << *it;
        }
    }
};

int main()
{
    vector<Order> orders;
    orders.push_back(Order("retek","A","C",30));
    orders.push_back(Order("eper","B","C",10));
    orders.push_back(Order("rebarbara","B","C",10));

    vector<Wagon> wagons;
    wagons.push_back(Wagon("a","A",10));
    wagons.push_back(Wagon("b","B",15));
    wagons.push_back(Wagon("c","A",10));
    wagons.push_back(Wagon("d","B",15));

    vector<string> vonatok;
    vonatok.push_back("vonat1schedule.txt");
    vonatok.push_back("vonat2schedule.txt");

    Database DB;
    if(!DB.init(vonatok,orders))
        cout<<"a macska rúgja meg!"<<endl;

    NodeSet<A> ns(DB);
    A kezdet(wagons,orders);
    // kezdet = ...
    ns.put(kezdet);
    bool megtalaltuk = false;
    int lepesszam=0;
    A legjobb = kezdet; // csak hogy legyen kezdeti ertek
    while(!megtalaltuk && ns.size() )
    {
        lepesszam++;
        A aktualis = ns.pop_max();
        if (vegeredmeny(aktualis,DB))
        {
            legjobb=aktualis;
            megtalaltuk=true;
        }
        vector<A> gyerekek = aktualis.gyerekek(DB);
        cout<<gyerekek.size()<<endl;
        for (A jelolt: gyerekek)
        {
            // if(..) //esetleg
            ns.put(jelolt);
            cout<<jelolt;
        }
    }
    if (megtalaltuk)
    {
        cout <<endl << endl << legjobb << " a vegeredmeny" << endl;
        cout << "(" << lepesszam << " lepes, eredmény history: " << legjobb.history.size() << ")" << endl;
    }
    else
    {
        cout << "nincs megoldás" << endl;
    }
    return 0;
}

