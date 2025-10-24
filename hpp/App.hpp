#ifndef APP_HPP
#define APP_HPP
#include "include.hpp"
#include "Flight.hpp"

class App
{
private:
    vector<shared_ptr<Flight>> flights;

public:
    App();
    string test(string in);
    vector<string> list_flights();
};

#endif