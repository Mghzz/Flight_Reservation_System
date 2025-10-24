#include "../hpp/App.hpp"

App ::App()
{
}

string App::test(string in)
{
    if (in == "Hello")
    {
        return "Hi , How are you ? :)";
    }
    else
    {
        return "Gave";
    }
}

vector<string> App::list_flights()
{
    vector<string> flights_info;

    for (int i = 0; i < sizeof(flights); i++)
    {
        flights_info.push_back(flights[i]->info());
    }
    return flights_info;
}
