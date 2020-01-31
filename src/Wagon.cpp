#include "Wagon.h"
#include <iostream>

using namespace std;

Wagon::Wagon(std::string name, std::string place, int size)
{
    _name=name;
    _place=place;
    _size=size;
    _train="";
}

void Wagon::pakol(std::string product,int quantity)
{
    if(_products.find(product)!=_products.end())
        _products[product]+=quantity;
    else _products[product]=quantity;
//    for(auto a:_products)
//        cout<<a.first<<" "<<a.second<<endl;
}

void Wagon::lepakol(std::string product,int quantity)
{

    auto it=_products.find(product);
    if(it!=_products.end())
    {
        if(it->second==quantity)
        {
            _products.erase(it);
        }
        else if(it->second>quantity)
            _products[product]-=quantity;
        else
            cout << "Hiba " << product << "lepakolása közben, " << _name <<
             " vagonról, nagyobb lepakolni kívánt mennyiség!"<<endl;
    }
    else
        cout << "Hiba " << product << "lepakolása közben, " << _name << " vagonról, nincs rajta a vagonon!"<<endl;
}

void Wagon::csatol(std::string train)
{
    _train=train;
}

void Wagon::lecsatol(std::string place)
{
    _place=place;
    _train="";
}

int Wagon::load()
{
    int res=0;
    for(auto a:_products)
        res+=a.second;
    return res;
}

bool Wagon::isOnTrain()
{
    if(_train.empty())
        return false;
    else
    {
        cout<<_train<<endl;
        return true;
    }
}

bool Wagon::operator<(const Wagon w) const
{
    if(_name!=w._name);
        return _name<w._name;
    if(_products!=w._products)
        return _products<w._products;
    return _train<w._train;
}

bool Wagon::operator!=(const Wagon w) const
{
    if(_name!=w._name);
        return true;
    if(_products!=w._products)
        return true;
    return _train!=w._train;
}
