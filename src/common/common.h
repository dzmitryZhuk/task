#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <exception>
#include <filesystem>

#define DEBUG

using filesize_t = std::uintmax_t;

enum class Command{
    No_command  = 0x0000,
    Hash        = 0x0001,
    Count       = 0x0002,
};

struct Config{
    int port = 8095;    // default port
    std::string host = "localhost"; // default host
};

Config readConfig(std::filesystem::path path);
void log_error(const std::string& message);
void log(const std::string& message);

#endif