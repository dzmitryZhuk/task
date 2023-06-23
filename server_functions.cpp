#include <sstream>
#include <vector>

#include "server_functions.h"

std::vector<std::vector<int>> L =   {{0x2, 0x3, 0x1, 0x1},
                                    {0x1, 0x2, 0x3, 0x1},
                                    {0x1, 0x1, 0x2, 0x3},
                                    {0x3, 0x1, 0x1, 0x2}};

int countWords(const std::string& text)
{
    std::stringstream ss(text);
    std::string word;
    int count = 0;

    while (ss >> word)
        count++;

    return count;
}

Command stringToCommand(const std::string& str)
{
    Command command;

    int temp = 0;
    auto ptr = str.c_str();

    for (int i = 0; i < sizeof(decltype(command)); ++i)
        temp |= static_cast<int>(*(ptr+i)) << (i * 8);
    command = static_cast<decltype(command)>(temp);

    return command;
}

std::string separateStrCommand(std::string& text)
{
    auto res = text.substr(0, sizeof(Command));
    //auto count = static_cast<std::size_t>(sizeof(Command));
    text.erase(0, sizeof(Command)); 
    return res;
}
