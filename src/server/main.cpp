#include <iostream>

#include "Server.h"

int main(int argc, char* argv[])
{
    if(argc > 1)
        if(std::string(argv[1]) == std::string("-h") || std::string(argv[1]) == std::string("--help"))
        {
            std::cout << "Usage:" << std::endl;
            std::cout << "  server [option]" << std::endl << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "    --cfg <path_to_config>        apply config file" << std::endl << std::endl;
            std::cout << "Structure of config file:" << std::endl << " <host>" << std::endl << " <port>" << std::endl    << std::endl;
            std::cout << " Usage example: server --cfg /user/config.txt" << std::endl;
            return 0;
        }

    Config cfg;
    if(argc > 1)
    {
        std::string opt1 = argv[1];
        if(opt1 != "--cfg")
            log_error(" wrong option");
        else if(argc <= 2)
            log_error(" config name required");
        else
            cfg = readConfig(argv[2]);
    }
    Server server(cfg.port);
    server.run();
    return 0;
}
