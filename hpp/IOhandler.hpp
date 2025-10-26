#ifndef IOHANDLER_HPP
#define IOHANDLER_HPP
#include "include.hpp"
#include "App.hpp"

#define PORT 8080
#define AIRLINES_PORT 9090
#define CUSTOMERS_PORT 9091
#define BUFFER_SIZE 1024

class App;
class IOHandler
{
private:
    App *app;
    int udp_socket_airline;
    int udp_socket_customer;
    struct sockaddr_in udp_airline_addr;
    struct sockaddr_in udp_customer_addr;
    std::unordered_map<int, string> client_usernames;

public:
    IOHandler(App *_app);
    void run_server();
    string handle_command(int fd, string command);
    void broadcast_to_airlines(string msg);
    void broadcast_to_customers(string msg);
};

#endif