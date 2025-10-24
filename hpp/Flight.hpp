#ifndef FLIGHT_HPP
#define FLIGHT_HPP
#include "include.hpp"

class Flight
{
private:
    string id;
    string origin;
    string destination;
    string time;
    vector<bool> seat_map;

public:
    Flight(string _id, string _origin, string _destination, string _time, int col, int row);
    string info();
};

#endif