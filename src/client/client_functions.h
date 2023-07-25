#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

#include "common.h"

filesize_t getFileSize(const std::filesystem::path& path); 
std::string readText(const std::filesystem::path& path);

#endif