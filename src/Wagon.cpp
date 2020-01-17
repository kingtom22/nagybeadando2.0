#include "Wagon.h"
#include <iostream>

using namespace std;

Wagon::Wagon(std::string name, std::string place, int size)
{
    _name=name;
    _place=place;
    _size=size;
}

void Wagon::pakol(std::string product,int quantity)
{
    if(_products.find(product)!=_products.end())
        _products[product]+=quantity;
    _products[product]=quantity;
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

int Wagon::load()
{
    int res=0;
    for(auto a:_products)
        res+=a.second;
    return res;
}
