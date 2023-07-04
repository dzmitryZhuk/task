#include <iostream>
#ifdef DEBUG
#include <cstdlib>
#endif

#include <boost/asio.hpp>

#include "client_functions.h"

int main(int argc, char* argv[])
{
    if(std::string(argv[1]) == std::string("--help"))
    {
        std::cout << " run with 2 arguments: [file path], [command];" << std::endl;
        std::cout << "   commands:" << std::endl;
        std::cout << "     -h - get hash of file text" << std::endl;
        std::cout << "     -c - count words of file text" << std::endl;
        return 0;
    }
    if(argc < 3)
    {
        std::cout << " 2 arguments required: file path and command" << std::endl;
        std::cout << " write argument --help for more information" << std::endl;
        return 0;
    }
    
    std::string path = argv[1];
    std::string com = argv[2];
    Command command = Command::No_command;

    if(com == "-h"){
        command = Command::Hash;        
    } else if(com == "-c"){
        command = Command::Count;
    }
    else{
        std::cout << " undefined command" << std::endl;
        return 0;
    }

    std::string text;
    try
    {
        text = readText(path);
    }
    catch(...)
    {
        std::cerr << " failed opening file "<< path << std::endl;
        return 0;
    }
    
    // sending
    auto host = "localhost";
    auto port = 8095;   // default port
    try{
        auto config_name = "config.txt";
        port = config_get_port(config_name);
    }
    catch(...){
        std::cerr << " failed opening config file; used default parameters" << std::endl;
    }
    try{
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::socket socket(ioContext);

        // connecting to server
        boost::asio::ip::tcp::resolver resolver(ioContext);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(socket, endpoints);
#ifdef DEBUG
    std::cout << " connected to server" << std::endl;
#endif

        // command sending
        boost::asio::write(socket, boost::asio::buffer(&command, sizeof(decltype(command))));
#ifdef DEBUG
    std::cout << " command <" << static_cast<int>(command) << "> sent" << std::endl;
#endif
        // file size sending
        auto size = getFileSize(path);
        boost::asio::write(socket, boost::asio::buffer(&size, sizeof(decltype(size))));
#ifdef DEBUG
    std::cout << " size <" << size << "> sent" << std::endl;
#endif
        // message sending
        boost::asio::write(socket, boost::asio::buffer(text));
#ifdef DEBUG
    std::cout << " data sent" << std::endl;
#endif

        // getting response
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n');
#ifdef DEBUG
    std::cout << " response received" << std::endl;
#endif
        std::istream input(&buffer);
        std::string response;
        std::getline(input, response);
        std::cout << " server response: ";
        if(command == Command::Count)
            std::cout << response;
        else
            for(const auto& i : response)
                std::cout << std::hex << static_cast<int>(i);
        std::cout << std::endl;
        
        socket.close();
    }
    catch(std::exception& e){
        std::cerr << " error: " << e.what() << std::endl;
    }

    return 0;
}