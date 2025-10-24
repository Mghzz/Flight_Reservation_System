#include <../hpp/include.hpp>
#include <../hpp/App.hpp>
#include <../hpp/IOhandler.hpp>

int main()
{
    App *FRS = new App();
    IOHandler handler(FRS);
    handler.handle_commands();
}
