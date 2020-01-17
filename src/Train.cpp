#include <algorithm>
#include "Train.h"

using namespace std;

Train::Train(std::string name,std::string place) : _name(name), _place(place) {}

void Train::csatol(std::string name)
{
    _wagons.push_back(name);
}

void Train::lecsatol(std::string name)
{
    _wagons.erase(find(_wagons.begin(),_wagons.end(),name));
}
