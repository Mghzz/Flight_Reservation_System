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
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i]->get_username() == username)
        {
            throw runtime_error(USERNAMEEXISTS);
        }
    }
    users.push_back(make_shared<User>(username, password, role));
}

void App::login(string username, string password)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i]->get_username() == username)
        {
            if (users[i]->get_password() == password)
            {
                users[i]->login();
                return;
            }
            else
            {
                throw runtime_error(INVALIDPASSWORD);
            }
        }
    }

    throw runtime_error(USERNOTFOUND);
}

void App::add_flight(string username, string id, string origin, string destination, string time, int col, int row)
{
    auto cur_user = find_user(username);
    if (!cur_user->logined())
    {
        throw runtime_error(USERNOTLOGINED);
    }
    if (cur_user->get_role() != AIRLINE)
    {
        throw runtime_error(PERMISSIONDENIED);
    }

    for (size_t i = 0; i < flights.size(); i++)
    {
        if (flights[i]->get_id() == id)
        {
            throw runtime_error(FLIGHTIDEXISTS);
        }
    }
    flights.push_back(make_shared<Flight>(id, origin, destination, time, col, row));
}

shared_ptr<User> App::find_user(string username)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i]->get_username() == username)
        {
            return users[i];
        }
    }
    throw runtime_error(USERNOTFOUND);
}