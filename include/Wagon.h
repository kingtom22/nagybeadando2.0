#ifndef WAGON_H
#define WAGON_H

#include <map>

class Wagon
{
    public:
        Wagon(std::string name, std::string place, int size);
        void mozgat(std::string place) { _place=place; }
        void pakol(std::string product,int quantity);
        void lepakol(std::string product,int quantity);
        bool isFull();
        int load();
        std::map<std::string,int> getProducts() { return _products; }
        std::string getName() { return _name; }
        std::string getPlace() { return _place; }
        int getSize() { return _size; }

        std::string _place;
    private:
        int _size;
        std::map<std::string,int> _products;
        std::string _name;

};

#endif // WAGON_H
