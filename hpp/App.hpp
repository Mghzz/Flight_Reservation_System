#ifndef APP_HPP
#define APP_HPP
#include "include.hpp"
#include "Flight.hpp"
#include "User.hpp"

class App
{
private:
    vector<shared_ptr<Flight>> flights;
    vector<shared_ptr<User>> users;

public:
    App();
    string test(string in);
    vector<string> list_flights();
    void user_register(string role, string username, string password);
    void login(string username, string password);
    void add_flight(string username, string id, string origin, string destination, string time, int col, int row);
    shared_ptr<User> find_user(string username);
};

#endif