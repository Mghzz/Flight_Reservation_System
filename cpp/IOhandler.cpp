#include "../hpp/IOhandler.hpp"

IOHandler::IOHandler(App *_app)
{
    app = _app;
}

void IOHandler::handle_commands()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    int opt = 1;
    socklen_t addrlen = sizeof(address);

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
        return;
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        return;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (new_socket < 0)
    {
        perror("accept failed");
        return;
    }

    std::cout << " Client connected!\n";

    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);

        if (bytes_read <= 0)
        {
            std::cout << " Client disconnected.\n";
            break;
        }

        std::string msg(buffer);
        string msg2 = app->test(msg);
        std::cout << " Received: " << msg << std::endl;

        if (msg == "exit" || msg == "EXIT")
        {
            std::cout << " Exit command received. Closing connection.\n";
            break;
        }

        std::string response = msg2;
        send(new_socket, response.c_str(), response.size(), 0);
    }

    close(new_socket);
    close(server_fd);
    std::cout << " Server closed.\n";
    return;
}
