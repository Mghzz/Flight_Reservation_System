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
using namespace std;

#ifndef INCLUDE_HPP
#define INCLUDE_HPP

const string AIRLINE = "airline";
const string CUSTOMER = "customer";
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