#include <fstream>
#include <filesystem>

#include "client_functions.h"

std::string readText(const std::string& path)
{
    std::ifstream file(path);
    std::string text;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)){
            line.push_back('\n');
            text += line;
        }
        file.close();
    } else {
        throw " failed reading text from file";
    }

    return text;    
}

filesize_t getFileSize(const std::string& path)
{
    // c++17 required
    return std::filesystem::file_size(path);
}
