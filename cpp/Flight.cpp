#include "../hpp/Flight.hpp"

Flight::Flight(int _id, string _origin, string _destination, string _time, int col, int row)
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