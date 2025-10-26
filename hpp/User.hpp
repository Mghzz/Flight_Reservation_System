#ifndef USER_HPP
#define USER_HPP
#include "include.hpp"

class User
{
private:
    string username;
    string password;
    string role;

public:
    User(string _username, string _password, string _role);
    string get_username();
};

#endif