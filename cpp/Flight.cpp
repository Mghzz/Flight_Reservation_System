#include "../hpp/Flight.hpp"

Flight::Flight(string _id, string _origin, string _destination, string _time, int col, int row)
{
    id = _id;
    origin = _origin;
    destination = _destination;
    time = _time;
    for (int i = 0; i < col * row; i++)
    {
        seat_map.push_back(FREE);
    }
}

string Flight::info()
{
    int available_seats = 0;
    int all_seats = sizeof(seat_map);
    for (int i = 0; i < all_seats; i++)
    {
        if (!seat_map[i])
        {
            available_seats++;
        }
    }
    return "FLIGHT " + id + " " + origin + " " + destination + " " + time + " " + "SEATS_AVAILABLE=" + to_string(available_seats) + "/" + to_string(all_seats);
}