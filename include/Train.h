#ifndef TRAIN_H
#define TRAIN_H

#include <vector>
#include <string>

class Train
{
    public:
        Train(std::string name,std::string place);
        void csatol(std::string name);
        void lecsatol(std::string name);

        std::string _name;
        std::string _place;
        std::vector<std::string> _wagons;
    private:

};

#endif // TRAIN_H
