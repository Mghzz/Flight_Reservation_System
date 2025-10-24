#ifndef IOHANDLER_HPP
#define IOHANDLER_HPP
#include "include.hpp"
#include "App.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024

class App;
class IOHandler
{
private:
    App *app;

public:
    IOHandler(App *_app);
    void handle_commands();
};

#endif