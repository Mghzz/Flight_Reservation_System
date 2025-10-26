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
};

#endif