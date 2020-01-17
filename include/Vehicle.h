#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle
{
    public:
        Vehicle();
        Vehicle(std::string name, std::string _place);

        std::string _name;
        std::string _place;

    protected:

    private:
};

#endif // VEHICLE_H
