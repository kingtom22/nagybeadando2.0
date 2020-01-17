#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

#include "Train.h"
#include "Database.h"

using namespace std;

struct A {
    map<string,map<string,int>> stations;//melyik állomáson mibõl mennyi van
    vector<Wagon> wagons;
    vector<Train> trains;// melyik vonat hol van
    vector<A> history;
    A(vector<Wagon> start_wagons,Database& db,vector<Order> orders)
    {
        wagons=start_wagons;
        map<string,vector<pair<string,int>>> scedules=db.GetScedules();
        for(auto t:scedules)
        {
            trains.push_back(Train(t.first,t.second.front().first));
        }
        for(Order& o:orders)
        {
            map<string,int> products=stations[o._source];
            products[o._name]+=o._quantity;
            stations[o._source]=products;
        }
    }
    vector<A> gyerekek(Database& db)
    {
        vector<A> res;
        if(history.empty())
        {
            for(size_t i=0;i<wagons.size();i++)
            {
                map<string,int> products=stations[wagons[i].getPlace()];
                for(pair<string,int> p:products)
                {
                    A uj(*this);
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
                        new_products[p.first]-=wagons[i].getSize()-wagons[i].load();
                        uj.stations[wagons[i].getPlace()]=new_products;
                        res.push_back(uj);
                    }
                }
            }
        }
        else
        {
            for(size_t i=0;i<wagons.size();i++)
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
                        new_products[p.first]-=wagons[i].getSize()-wagons[i].load();
                        uj.stations[wagons[i].getPlace()]=new_products;
                        res.push_back(uj);
                    }
                }

            }
            A uj(*this);
            uj.history.push_back(uj);
            res.push_back(uj);
            for(size_t i=0;i<trains.size();i++)
            {
                string place=db.whereIsThisTrain(trains[i]._name,uj.history.size());
                vector<Wagon> station_wagons;
                copy_if(wagons.begin(),wagons.end(),back_inserter(station_wagons),
                        [place](const Wagon& w)
                {
                    return w._place==place;
                });
                for(Wagon w:station_wagons)
                {
                    uj.trains[i].csatol(w.getName());
                }
            }
        }
        return res;
    }
};
float value(const A& a) {
    return 1; //TODO
}
bool operator<(const A& a, const A& b) {
    return value(a) < value(b);
}

bool vegeredmeny(const A& a,Database& db) {
    return db.hasOrdersArrived(a.stations);
}
ostream & operator<<(ostream &out, const A& a) {
    for(size_t t=0;t<a.history.size();t++)
    {
        cout << t << " idõpontban"<<endl;
        for(auto station:a.history[t].stations)
        {
            cout<<station.first<<endl;
            for(Wagon w:a.history[t].wagons)
            {
                if(w.getPlace()==station.first)
                {
                    cout << w.getName() << endl;
                    for(auto product:w.getProducts())
                        cout << "\t" << product.first << "\t" << product.second <<endl;
                }
            }
            cout<< "lepakolva:" << endl;
            for(auto product:station.second)
                cout << "\t" << product.first << "\t" << product.second <<endl;
        }
    }
    cout<<"----------------------------"<<endl;

    return out;
}
template<typename T>
class NodeSet {
    set<T> t;
public:
    T pop_max() {
        auto it_max = t.begin();
        for (auto it=t.begin();it!=t.end();it++) {
            if (value(*it) > value(*it_max)) {
                it_max = it;
            }
        }
        T res = *it_max;
        t.erase(it_max);
        return res;
    }
    void put(T a) {
        t.insert(a);
    }
    int size() const {
        return t.size();
    }
    void dump(ostream &out = cout) {
        for (auto it=t.begin();it!=t.end();it++) {
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

    vector<string> vonatok;
    vonatok.push_back("vonat1schedule.txt");
    vonatok.push_back("vonat2schedule.txt");

    Database DB;
    if(!DB.init(vonatok))
        cout<<"a macska rúgja meg!"<<endl;
    DB.SetOrders(orders);

    NodeSet<A> ns;
    A kezdet(wagons,DB,orders);
    // kezdet = ...
    ns.put(kezdet);
    ns.dump();
    bool megtalaltuk = false;
    int lepesszam=0;
    A legjobb = kezdet; // csak hogy legyen kezdeti ertek
    while(!megtalaltuk && ns.size() ) {
        lepesszam++;
        A aktualis = ns.pop_max();
        if (vegeredmeny(aktualis,DB)) {
            legjobb=aktualis;
            megtalaltuk=true;
        }
        vector<A> gyerekek = aktualis.gyerekek(DB);
        for (A jelolt: gyerekek) {
            // if(..) //esetleg
            ns.put(jelolt);
        }
    }
    if (megtalaltuk) {
        cout <<endl << endl << legjobb << " a vegeredmeny" << endl;
        cout << "(" << lepesszam << " lepes, eredmény history: " << legjobb.history.size() << ")" << endl;
    } else {
        cout << "nincs megoldas" << endl;
    }
    return 0;
}

