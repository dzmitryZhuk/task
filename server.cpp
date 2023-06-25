// #include <iostream>
// #include <fstream>
// #include <thread>

// #include <boost/asio.hpp>

// #include "server_functions.h"

// void clientHandler(boost::asio::ip::tcp::socket socket) {
// #ifdef DEBUG
//     static int client_count = 0;
//     client_count++;
//     std::cout << " client connection №" << client_count << std::endl;
// #endif
//     try {
//         // reading client data
//         boost::asio::streambuf buffer;
//         boost::asio::read_until(socket, buffer, '\n');
//         std::istream input(&buffer);
//         std::string message;
//         std::getline(input, message);
// #ifdef DEBUG
//         std::cout << " message from client: " << message << std::endl;
// #endif
//         std::string response;
//         auto command = stringToCommand(separateStrCommand(message));
//         switch (command)
//         {
//         case Count:
//             response = std::to_string(countWords(message));
//             break;
//         case Hash:
//             /////////////////////////////////////////////////////////////////////////////
//             break;
//         case No_command:
//         default:
//             std::cerr << " error command" << std::endl;
//             break;
//         }
//         response += '\n';
//         // sending response
// #ifdef DEBUG
//         std::cout << " sending response: " << response << std::endl;
// #endif
//         boost::asio::write(socket, boost::asio::buffer(response));
//     } catch (std::exception& e) {
//         std::cerr << " error: " << e.what() << std::endl;
//     }
// }

// int main(int argc, char* argv[])
// {
//     auto port = 8095;   // default port
//     try{
//         auto config_name = "config.txt";
//         port = config_get_port(config_name);
//     }
//     catch(...){
//         std::cerr << " failed opening config file; used default parameters" << std::endl;
//     }

//     //
//     try {
//         boost::asio::io_context ioContext;
//         boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
// #ifdef DEBUG
//         std::cout << " start working" << std::endl;
// #endif
//         while (true) {
//             boost::asio::ip::tcp::socket socket(ioContext);
//             acceptor.accept(socket);
            
//             // start client handler in new thread
//             std::thread thread(clientHandler, std::move(socket));
//             thread.detach();
//         }
//     } catch (std::exception& e) {
//         std::cerr << " error: " << e.what() << std::endl;
//     }
//     return 0;
// }