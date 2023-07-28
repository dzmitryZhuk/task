#include <iostream>
#ifdef DEBUG
#include <cstdlib>
#endif

#include "Client.h"

int main(int argc, char* argv[])
{
    if(argc < 2 || argc > 3)
    {
        std::cout << " 2 arguments required: file path and command" << std::endl;
        std::cout << " write argument -h or --help for more information" << std::endl;
        return 0;
    }
    if(std::string(argv[1]) == std::string("-h") || std::string(argv[1]) == std::string("--help"))
    {
        std::cout << " run with 2 arguments: [file path], [command];" << std::endl;
        std::cout << "   commands:" << std::endl;
        std::cout << "     --hash - get hash of file text" << std::endl;
        std::cout << "     --count - count words of file text" << std::endl;
        std::cout << " Example <client /home/file_with_text.txt --count>" << std::endl;
        return 0;
    }
    
    std::filesystem::path path = argv[1];
    std::string com = argv[2];
    Command command = Command::No_command;

    if(com == "--hash")
        command = Command::Hash;        
    else if(com == "--count")
        command = Command::Count;
    else{
        std::cout << " undefined command" << std::endl;
        return 0;
    }

    auto cfg = readConfig("config.txt");
    
    Client client;
    if(!client.connect(cfg.host, cfg.port))
        return 0;
    if(!client.sendText(path, command))
        return 0;

    auto response = client.getResponse();
    client.disconnect();
    std::cout << " server response: ";
    if(command == Command::Count)
        std::cout << response;
    else
        for(const auto& i : response)
            std::cout << std::hex << (static_cast<int>(i) & 0xff);
    std::cout << std::endl;

    return 0;
}