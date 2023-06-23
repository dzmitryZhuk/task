#include <fstream>

#include "client_functions.h"

std::string readText(const std::string& path)
{
    std::ifstream file(path);
    std::string text;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line))
            text += line + "\n";
        file.close();
    } else {
        throw 1;
    }

    return text;    
}

std::string addCommand(std::string& text, const Command& command)
{
    auto strCommand = commandToString(command);
    text.insert(0, strCommand);
    return text;
}

std::string commandToString(Command command)
{
    std::string res;
    
    auto bytePointer = reinterpret_cast<char*>(&command);

    for (std::size_t i = 0; i < sizeof(decltype(command)); ++i)
        res += static_cast<char>(bytePointer[i]);

    return res;
}
