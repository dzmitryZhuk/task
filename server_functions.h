#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <vector>

#include "common.h"

Command stringToCommand(const std::string& str);    // converts the bytes of char to Command
std::string separateStrCommand(std::string& text);  // separate command(bytes of char) from text and returns command
int countWords(const std::string& text);

#endif