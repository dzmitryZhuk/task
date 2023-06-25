#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>

#define DEBUG

enum Command{
    No_command  = 0x0000,
    Hash        = 0x0001,
    Count       = 0x0002,
};

int config_get_port(const std::string& config_path);

#endif