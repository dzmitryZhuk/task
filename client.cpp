// #include <iostream>
// #ifdef DEBUG
// #include <cstdlib>
// #endif

// #include <boost/asio.hpp>

// #include "client_functions.h"

// int main(int argc, char* argv[])
// {
//     // if(argc < 2)
//     // {
//     //     std::cout << " 2 arguments required: file path and command" << std::endl;
//     //     std::cout << " write argument -h for more information" << std::endl;
//     // }


    
//     std::string path = argv[1];
//     std::string com = argv[2];
//     Command command = No_command;

//     if(com == "-h"){
//         command = Hash;        
//     } else if(com == "-c"){
//         command = Count;
//     }
//     else{
//         std::cout << " undefined command" << std::endl;
//         return 0;
//     }

//     std::string text;
//     try
//     {
//         text = readText(path);
//     }
//     catch(...)
//     {
//         std::cerr << " failed opening file "<< path << std::endl;
//         return 0;
//     }
// #if defined(DEBUG) && defined(__linux__)
//     std::string cc = "cat ";
//     cc += path;
//     std::system(cc.c_str());
//     std::cout << std::endl;
// #endif
//     addCommand(text, command);
    
//     // sending
//     auto host = "localhost";
//     auto port = 8095;   // default port
//     try{
//         auto config_name = "config.txt";
//         port = config_get_port(config_name);
//     }
//     catch(...){
//         std::cerr << " failed opening config file; used default parameters" << std::endl;
//     }
//     try{
//         boost::asio::io_context ioContext;
//         boost::asio::ip::tcp::socket socket(ioContext);

//         // connecting to server
//         boost::asio::ip::tcp::resolver resolver(ioContext);
//         boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
//         boost::asio::connect(socket, endpoints);
// #ifdef DEBUG
//     std::cout << " connected to server" << std::endl;
// #endif

//         // message sending
//         boost::asio::write(socket, boost::asio::buffer(text));
// #ifdef DEBUG
//     std::cout << " data sent" << std::endl;
// #endif

//         // getting response
//         boost::asio::streambuf buffer;
//         boost::asio::read_until(socket, buffer, '\n');
// #ifdef DEBUG
//     std::cout << " response received" << std::endl;
// #endif
//         std::istream input(&buffer);
//         std::string response;
//         std::getline(input, response);
//         std::cout << " server response: " << response << std::endl;
        
//         socket.close();
//     }
//     catch(std::exception& e){
//         std::cerr << " error: " << e.what() << std::endl;
//     }

//     return 0;
// }