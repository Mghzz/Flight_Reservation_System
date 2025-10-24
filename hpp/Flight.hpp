#ifndef FLIGHT_HPP
#define FLIGHT_HPP
#include "include.hpp"

class Flight
{
private:
    int id;
    string origin;
    string destination;
    string time;
    vector<bool> seat_map;

public:
    Flight(int _id, string _origin, string _destination, string _time, int col, int row);
};

#endif