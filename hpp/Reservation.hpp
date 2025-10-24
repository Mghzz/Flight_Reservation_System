#ifndef RESERVATION_HPP
#define RESERVATION_HPP
#include "include.hpp"

class Reservation
{
private:
    int id;
    int flight_id;
    int timestamp;
    string username;
    string status;
    vector<string> seats;

public:
    Reservation(int _id, int flight_id, int timestamp, string _username, string _status, vector<string> seats);
};

#endif