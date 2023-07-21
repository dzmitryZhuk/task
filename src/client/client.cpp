#include <iostream>
#ifdef DEBUG
#include <cstdlib>
#endif

#include <boost/asio.hpp>

#include "client_functions.h"

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
    
    std::string path = argv[1];
    std::string com = argv[2];
    Command command = Command::No_command;

    if(com == "--hash"){
        command = Command::Hash;        
    } else if(com == "--count"){
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
    catch (const std::exception& e) {
        std::cerr << " failed reading text from file" << std::endl;
        std::cerr << " error: " << e.what() << std::endl;
        return 0;
    }
    catch(const char* message){
        std::cerr << message << std::endl;
        return 0;
    }
    
    // sending
    auto cfg = readConfig("config.txt");
    try{
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::socket socket(ioContext);

        // connecting to server
        boost::asio::ip::tcp::resolver resolver(ioContext);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(cfg.host, std::to_string(cfg.port));
        boost::asio::connect(socket, endpoints);
        log(" connected to server");

        // command sending
        boost::asio::write(socket, boost::asio::buffer(&command, sizeof(decltype(command))));
        log(std::string(" command <") + std::to_string(static_cast<int>(command)) + std::string("> sent"));
        // file size sending
        auto size = getFileSize(path);
        boost::asio::write(socket, boost::asio::buffer(&size, sizeof(decltype(size))));
        log(std::string(" size <") + std::to_string(size) + std::string("> sent"));
        // message sending
        boost::asio::write(socket, boost::asio::buffer(text));
        log(" data sent");

        // getting response
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n');
        log(" response received");
        std::istream input(&buffer);
        std::string response;
        std::getline(input, response);
        std::cout << " server response: ";
        if(command == Command::Count)
            std::cout << response;
        else
            for(const auto& i : response)
                std::cout << std::hex << (static_cast<int>(i) & 0xff);
        std::cout << std::endl;
        
        socket.close();
    }
    catch(const std::exception& e){
        std::cerr << " error: " << e.what() << std::endl;
    }
    catch(const char* message){
        std::cerr << message << std::endl;
    }

    return 0;
}