#include "../hpp/User.hpp"

User::User(string _username, string _password, string _role)
{
    username = _username;
    password = _password;
    role = _role;
}

string User::get_username()
{
    return username;
}