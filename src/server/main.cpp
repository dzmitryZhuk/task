#include <iostream>

#include "Server.h"

int main(int argc, char* argv[])
{
    auto cfg = readConfig("config.txt");
    Server server(cfg.port);
    server.run();
    return 0;
}
