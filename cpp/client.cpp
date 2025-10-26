#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

#define PORT 8080
#define BUFFER_SIZE 1024
#define AIRLINES_PORT 9090
#define CUSTOMERS_PORT 9091
const string AIRLINE = "airline";
const string CUSTOMER = "customer";

vector<string> split_string(const string &str, char delimiter)
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

void start_udp_listener(string role)
{
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0)
    {
        perror("UDP socket creation failed");
        return;
    }

    int reuse = 1;
    setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
#ifdef SO_REUSEPORT
    setsockopt(udp_sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
#endif

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(role == AIRLINE ? AIRLINES_PORT : CUSTOMERS_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udp_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("UDP bind failed");
        close(udp_sock);
        return;
    }

    cout << "Listening for UDP broadcasts on port "
         << (role == AIRLINE ? AIRLINES_PORT : CUSTOMERS_PORT)
         << "...\n";

    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        ssize_t n = recvfrom(udp_sock, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
        if (n > 0)
        {
            cout << "\n [Broadcast] " << buffer << endl;
            cout << "> " << flush;
        }
    }
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection failed");
        return 1;
    }

    cout << "Connected to server! Type your commands (type 'exit' to quit)\n";
    cout << "Example REGISTER: REGISTER AIRLINE Ali 123\n";

    string role;
    bool is_registered = false;
    thread udp_thread;

    while (true)
    {
        string msg;
        cout << "> ";
        getline(cin, msg);

        send(sock, msg.c_str(), msg.size(), 0);

        if (msg == "exit" || msg == "EXIT")
        {
            cout << "Disconnecting...\n";
            break;
        }

        if (!is_registered)
        {
            vector<string> tokens;
            tokens = split_string(msg, ' ');

            if (tokens.size() == 4 && tokens[0] == "REGISTER")
            {
                role = tokens[1];
                is_registered = true;
                udp_thread = thread(start_udp_listener, role);
                udp_thread.detach();
            }
        }

        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE - 1);
        if (valread <= 0)
        {
            cout << "Server closed connection.\n";
            break;
        }

        cout << "Server: " << buffer << endl;
    }

    close(sock);
    cout << "Client closed.\n";
    return 0;
}
