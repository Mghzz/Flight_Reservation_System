#ifndef RESERVATION_HPP
#define RESERVATION_HPP
#include "include.hpp"

class Reservation
{
private:
    string id;
    string flight_id;
    int timestamp;
    string username;
    string status;
    vector<string> seats;

public:
    Reservation(string _id, string flight_id, int timestamp, string _username, string _status, vector<string> seats);
};

#endif