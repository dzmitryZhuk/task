#include <fstream>
#include <iostream>

#include "common.h"

Config readConfig(std::filesystem::path path)
{
    Config config;
    std::ifstream configFile(path);
    if(configFile.is_open())
    {
        configFile >> config.host;
        configFile >> config.port;
    }
    else
    {
        std::cerr << " failed opening config file" << std::endl;
    }
    return config;
}

void log(const std::string& message)
{
#ifdef DEBUG
    std::cout << message << std::endl;
#endif
}
