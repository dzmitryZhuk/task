#include <iostream>
#ifdef DEBUG
#include <cstdlib>
#endif

#include "Client.h"
#include "common.h"

int main(int argc, char* argv[])
{
    if(argc < 2 || argc > 5)
    {
        std::cout << " 2 arguments required: file path and command" << std::endl;
        std::cout << " write argument -h or --help for more information" << std::endl;
        return 0;
    }
    if(std::string(argv[1]) == std::string("-h") || std::string(argv[1]) == std::string("--help"))
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "  client <filename> <command> [optional options]" << std::endl << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  <filename>                      path to file, that must be sent to server" << std::endl;
        std::cout << "  <command>:" << std::endl;
        std::cout << "    --hash                        get hash of file's text" << std::endl;
        std::cout << "    --count                       count words of file's text" << std::endl;
        std::cout << "    --cfg <path_to_config>        apply config file" << std::endl << std::endl;
        std::cout << "Structure of config file:" << std::endl << " <host>" << std::endl << " <port>" << std::endl    << std::endl;
        std::cout << " Usage example: client /home/file_with_text.txt --count --cfg /user/config.txt" << std::endl;
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
        log_error(" undefined command <" + com + ">");
        return 0;
    }

    Config cfg;
    if(argc > 3)
    {
        std::string opt1 = argv[3];
        if(opt1 != "--cfg")
            log_error(" wrong option");
        if(argc <= 4)
            log_error(" config name required");
        else
            cfg = readConfig(argv[4]);
    }

    Client client;
    if(!client.connect(cfg.host, cfg.port))
        return 0;
    if(!client.sendText(path, command))
        return 0;

    auto response = client.getResponse();
    log(" server response: ");
    if(command == Command::Count)
        std::cout << response;
    else
        for(const auto& i : response)
            std::cout << std::hex << (static_cast<int>(i) & 0xff);
    std::cout << std::endl;

    client.disconnect();

    return 0;
}