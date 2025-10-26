#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <algorithm>
#include <errno.h>
using namespace std;

#ifndef INCLUDE_HPP
#define INCLUDE_HPP

const string AIRLINE = "airline";
const string CUSTOMER = "customer";
const string USERNAMEEXISTS = "UsernameAlreadyExists";
const string NOTENOUGHARGS = "NotEnoughArgs";
const string LIST_FLIGHTS = "LIST_FLIGHTS";
const string REGISTER = "REGISTER";
const string REGISTERED_OK = "REGISTERED OK";
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