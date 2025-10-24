#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (serv_addr.sin_addr.s_addr == INADDR_NONE)
    {
        perror("Invalid address");
        return 1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    std::cout << "✅ Connected to server! Type messages (type 'exit' to quit):\n";

    while (true)
    {
        std::string msg;
        std::cout << "> ";
        std::getline(std::cin, msg);

        send(sock, msg.c_str(), msg.size(), 0);

        if (msg == "exit" || msg == "EXIT")
        {
            std::cout << "🛑 Disconnecting...\n";
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE - 1);
        if (valread <= 0)
        {
            std::cout << "❌ Server closed connection.\n";
            break;
        }

        std::cout << "📩 Server: " << buffer << std::endl;
    }

    close(sock);
    std::cout << "🔴 Client closed.\n";
    return 0;
}
