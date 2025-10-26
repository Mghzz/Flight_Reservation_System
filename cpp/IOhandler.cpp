#include <IOhandler.hpp>

IOHandler::IOHandler(App *_app)
{
    app = _app;
}

void IOHandler::run_server()
{
    int server_fd, new_socket, max_sd;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    std::vector<int> clients;

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

    std::cout << "Server listening on port " << PORT << "...\n";

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
            std::cout << "New client connected (fd=" << new_socket << ")\n";
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
                    std::cout << "Client disconnected (fd=" << sd << ")\n";
                    close(sd);
                    it = clients.erase(it);
                    continue;
                }

                std::string msg(buffer);
                msg.erase(std::remove(msg.begin(), msg.end(), '\n'), msg.end());
                msg.erase(std::remove(msg.begin(), msg.end(), '\r'), msg.end());

                std::cout << "Received from fd=" << sd << ": " << msg << std::endl;

                if (msg == "exit" || msg == "EXIT")
                {
                    std::cout << "Closing connection for fd=" << sd << "\n";
                    close(sd);
                    it = clients.erase(it);
                    continue;
                }

                std::string response;
                try
                {
                    response = handle_command(msg);
                }
                catch (std::runtime_error &e)
                {
                    response = std::string("ERROR ") + e.what();
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
    std::cout << "Server stopped.\n";
}

string IOHandler::handle_command(string command)
{

    vector<string> tokens = split_string(command, ' ');

    if (tokens[0] == LIST_FLIGHTS)
    {
        vector<string> list = app->list_flights();
        string whole_list = "";
        for (int i = 0; i < list.size(); i++)
        {
            whole_list += list[i];
            if (i != list.size() - 1)
            {
                whole_list += "\n";
            }
        }
        return whole_list;
    }
    else if (tokens[0] == REGISTER)
    {
        if (tokens.size() < 4)
        {
            throw runtime_error(NOTENOUGHARGS);
        }
        app->user_register(tokens[1], tokens[2], tokens[3]);
        return REGISTERED_OK;
    }
}