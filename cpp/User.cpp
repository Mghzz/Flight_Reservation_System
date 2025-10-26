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

string User::get_password()
{
    return password;
}

string User::get_role()
{
    return role;
}

void User::login()
{
    is_logined = 1;
}

bool User::logined()
{
    return is_logined;
}