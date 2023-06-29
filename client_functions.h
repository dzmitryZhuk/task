#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

#include "common.h"

filesize_t getFileSize(const std::string& path); 
std::string readText(const std::string& path);  // reads text from file
// std::string addCommand(std::string& text, const Command& command);  // adds command in begin of text
// std::string commandToString(Command command);   // create string from command (needs for function addCommand)

#endif