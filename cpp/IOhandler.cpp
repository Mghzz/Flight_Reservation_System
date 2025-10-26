#include "../hpp/IOhandler.hpp"

IOHandler::IOHandler(App *_app)
{
    app = _app;
    udp_socket_airline = -1;
    udp_socket_customer = -1;
}

void IOHandler::run_server()
{
    int server_fd, new_socket, max_sd;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    vector<int> clients;

    // TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket failed");
        return;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        close(server_fd);
        return;
    }

    cout << "Server listening on port " << PORT << "...\n";

    // AIRLINES UDP
    udp_socket_airline = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket_airline < 0)
    {
        perror("UDP airline socket creation failed");
    }
    else
    {
        int broadcastEnable = 1;
        setsockopt(udp_socket_airline, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

        memset(&udp_airline_addr, 0, sizeof(udp_airline_addr));
        udp_airline_addr.sin_family = AF_INET;
        udp_airline_addr.sin_port = htons(AIRLINES_PORT);
        udp_airline_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        cout << "UDP Broadcast ready for airlines on port 9090\n";
    }

    // CUSTOMERS UDP
    udp_socket_customer = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket_customer < 0)
    {
        perror("UDP customer socket creation failed");
    }
    else
    {
        int broadcastEnable = 1;
        setsockopt(udp_socket_customer, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

        memset(&udp_customer_addr, 0, sizeof(udp_customer_addr));
        udp_customer_addr.sin_family = AF_INET;
        udp_customer_addr.sin_port = htons(CUSTOMERS_PORT);
        udp_customer_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        cout << "UDP Broadcast ready for customers on port 9091\n";
    }

    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        for (int sd : clients)
        {
            FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR)
        {
            perror("select error");
            continue;
        }

        if (FD_ISSET(server_fd, &readfds))
        {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0)
            {
                perror("accept failed");
                continue;
            }

            clients.push_back(new_socket);
            cout << "New client connected (fd=" << new_socket << ")\n";
        }

        for (auto it = clients.begin(); it != clients.end();)
        {
            int sd = *it;

            if (FD_ISSET(sd, &readfds))
            {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_read = recv(sd, buffer, BUFFER_SIZE - 1, 0);

                if (bytes_read <= 0)
                {
                    cout << "Client disconnected (fd=" << sd << ")\n";
                    close(sd);
                    it = clients.erase(it);
                    continue;
                }

                string msg(buffer);
                msg.erase(remove(msg.begin(), msg.end(), '\n'), msg.end());
                msg.erase(remove(msg.begin(), msg.end(), '\r'), msg.end());

                cout << "Received from fd=" << sd << ": " << msg << endl;

                if (msg == "exit" || msg == "EXIT")
                {
                    cout << "Closing connection for fd=" << sd << "\n";
                    close(sd);
                    it = clients.erase(it);
                    continue;
                }

                string response;
                try
                {
                    response = handle_command(sd, msg);
                }
                catch (runtime_error &e)
                {
                    response = string("ERROR ") + e.what();
                }

                response += '\n';
                send(sd, response.c_str(), response.size(), 0);
            }

            ++it;
        }
    }

    for (int sd : clients)
        close(sd);
    close(server_fd);
    cout << "Server stopped.\n";
}

string IOHandler::handle_command(int fd, string command)
{
    vector<string> tokens = split_string(command, ' ');

    if (tokens.empty())
        throw runtime_error(UNKNOWN_COMMAND);

    if (tokens[0] == LIST_FLIGHTS)
    {
        if (client_usernames.find(fd) == client_usernames.end())
            throw runtime_error(USERNOTREGISTERED);
        vector<string> list = app->list_flights();
        string whole_list;
        for (int i = 0; i < list.size(); i++)
        {
            whole_list += list[i];
            if (i != list.size() - 1)
                whole_list += "\n";
        }
        return whole_list;
    }
    else if (tokens[0] == REGISTER)
    {
        if (tokens.size() < 4)
            throw runtime_error(NOTENOUGHARGS);
        if (client_usernames.find(fd) != client_usernames.end())
            throw runtime_error(USERALREADYREGISTERED);

        string role = tokens[1];
        string username = tokens[2];
        string password = tokens[3];
        app->user_register(role, username, password);

        client_usernames[fd] = username;

        string msg = "BROADCAST NEW_USER " + username + " " + role;
        broadcast_to_airlines(msg);

        return REGISTERED_OK;
    }

    else if (tokens[0] == "LOGIN")
    {
        if (tokens.size() < 3)
            throw runtime_error(NOTENOUGHARGS);
        if (client_usernames.find(fd) == client_usernames.end())
            throw runtime_error(USERNOTREGISTERED);
        app->login(tokens[1], tokens[2]);
        return LOGIN_OK;
    }

    else if (tokens[0] == "ADD_FLIGHT")
    {
        if (client_usernames.find(fd) == client_usernames.end())
            throw runtime_error(USERNOTREGISTERED);
        if (tokens.size() < 7)
            throw runtime_error(NOTENOUGHARGS);

        string username = client_usernames[fd];
        app->add_flight(username, tokens[1], tokens[2], tokens[3], tokens[4], stoi(tokens[5]), stoi(tokens[6]));

        string msg = "BROADCAST NEW_FLIGHT " + tokens[1] + " " + tokens[2] + " " + tokens[3] + " " + tokens[4];
        broadcast_to_customers(msg);
        return FLIGHT_ADDED_OK;
    }

    return UNKNOWN_COMMAND;
}

void IOHandler::broadcast_to_airlines(string msg)
{
    if (udp_socket_airline >= 0)
    {
        sendto(udp_socket_airline, msg.c_str(), msg.size(), 0,
               (struct sockaddr *)&udp_airline_addr, sizeof(udp_airline_addr));
    }
}

void IOHandler::broadcast_to_customers(string msg)
{
    if (udp_socket_customer >= 0)
    {
        sendto(udp_socket_customer, msg.c_str(), msg.size(), 0,
               (struct sockaddr *)&udp_customer_addr, sizeof(udp_customer_addr));
    }
}
