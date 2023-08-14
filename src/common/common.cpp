#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

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

void log_error(const std::string& message)
{
    std::cerr << message << std::endl;
}

void log(const std::string& message)
{
#ifdef DEBUG
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&now), "%F %T");
    std::cout << message << std::endl;
#endif
}
