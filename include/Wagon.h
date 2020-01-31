#ifndef WAGON_H
#define WAGON_H

#include <map>

class Wagon
{
    public:
        Wagon(std::string name, std::string place, int size);
        void pakol(std::string product,int quantity);
        void lepakol(std::string product,int quantity);

        void csatol(std::string train);
        void lecsatol(std::string place);

        bool isOnTrain();
        int load();

        std::map<std::string,int> getProducts() { return _products; }
        std::string getName() { return _name; }
        std::string getPlace() { return _place; }
        std::string getTrain() { return _train; }
        int getSize() { return _size; }

        bool operator<(const Wagon w) const;
        bool operator!=(const Wagon w) const;

    private:
        int _size;
        std::map<std::string,int> _products;
        std::string _name;
        std::string _place;
        std::string _train;

};

#endif // WAGON_H
