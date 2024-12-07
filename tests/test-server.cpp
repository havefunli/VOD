#include "../include/Server.h"
#include <iostream>

int main() 
{
    spdlog::set_level(spdlog::level::debug);
    Server srv(8888);
    srv.Start();
    
    return 0;
}

