#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

#include "common.h"

filesize_t getFileSize(const std::string& path); 
std::string readText(const std::string& path);  // reads text from file

#endif