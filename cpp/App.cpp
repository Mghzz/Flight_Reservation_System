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

    for (int i = 0; i < flights.size(); i++)
    {
        flights_info.push_back(flights[i]->info());
    }
    return flights_info;
}

void App::user_register(string role, string username, string password)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->get_username() == username)
        {
            throw runtime_error(USERNAMEEXISTS);
        }
    }
    users.push_back(make_shared<User>(username, password, role));
}
