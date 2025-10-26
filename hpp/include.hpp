#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <errno.h>
using namespace std;

#ifndef INCLUDE_HPP
#define INCLUDE_HPP

const string AIRLINE = "airline";
const string CUSTOMER = "customer";
const string USERNAMEEXISTS = "UsernameAlreadyExists";
const string FLIGHTIDEXISTS = "FlightIDAlreadyExists";
const string USERALREADYREGISTERED = "UserAlreadyRegistered";
const string NOTENOUGHARGS = "NotEnoughArgs";
const string LIST_FLIGHTS = "LIST_FLIGHTS";
const string REGISTER = "REGISTER";
const string REGISTERED_OK = "REGISTERED OK";
const string LOGIN_OK = "LOGIN OK";
const string FLIGHT_ADDED_OK = "FLIGHT_ADDED OK";
const string INVALIDPASSWORD = "InvalidPassword";
const string USERNOTFOUND = "UserNotFound";
const string USERNOTREGISTERED = "UserNotRegistered";
const string USERNOTLOGINED = "UserNotLogined";
const string PERMISSIONDENIED = "PermissionDenied";
const string UNKNOWN_COMMAND = "UNKNOWN_COMMAND";
const bool FREE = 0;
const bool RESERVED = 1;

inline vector<string> split_string(const string &str, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream sstr(str);
    while (getline(sstr, token, delimiter))
    {
        tokens.push_back(move(token));
    }
    return tokens;
}

#endif