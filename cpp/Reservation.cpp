#include "../hpp/Reservation.hpp"

Reservation::Reservation(string _id, string _flight_id, int _timestamp, string _username, string _status, vector<string> _seats)
{
    id = _id;
    flight_id = _flight_id;
    timestamp = _timestamp;
    username = _username;
    status = _status;
    for (int i = 0; i < sizeof(seats); i++)
    {
        seats.push_back(_seats[i]);
    }
}