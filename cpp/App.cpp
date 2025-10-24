#include "../hpp/App.hpp"

App ::App()
{
}

string App::test(string in)
{
    if (in == "Hello")
    {
        return "Hi , How are you ? :)";
    }
    else
    {
        return "Gave";
    }
}
