#include <iostream>
#include <fstream>
#include <thread>

#include <boost/asio.hpp>

#include "server_functions.h"
#include "common.h"

std::string make_string(const boost::asio::streambuf& streambuf)
{
  return {boost::asio::buffers_begin(streambuf.data()), 
          boost::asio::buffers_end(streambuf.data())};
}

void clientHandler(boost::asio::ip::tcp::socket socket) {
#ifdef DEBUG
    static int client_count = 0;
    client_count++;
    std::cout << " client connection №" << client_count << std::endl;
#endif
    try {
        // reading client data
        boost::asio::streambuf buffer;

        // reading command
        auto bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(sizeof(Command)));
        if(bytes_received != sizeof(Command)) throw "error reading Command";

        const Command* command_ptr = boost::asio::buffer_cast<const Command*>(buffer.data());
        auto command = *command_ptr;
        //command = stringToCommand(make_string(buffer));
        buffer.consume(bytes_received);

        // reading file size
        filesize_t file_size;
        bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(sizeof(filesize_t)));
        if(bytes_received != sizeof(filesize_t)) throw "error reading file size";

        const filesize_t* size_ptr = boost::asio::buffer_cast<const filesize_t*>(buffer.data());
        auto size = *size_ptr;
        buffer.consume(bytes_received);

        // read text
        std::string message;
        message.reserve(size);
        bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(size));
        if(bytes_received != size) throw "error reading file";
        const char* data = boost::asio::buffer_cast<const char*>(buffer.data());
        message.append(data, size);

        std::string response;
        switch (command)
        {
        case Count:{
            response = std::to_string(countWords(message));
            break;
        }
        case Hash:{
            auto temp = countHash(message);
            response = std::string(temp.begin(), temp.end());
            break;
        }
        case No_command:
        default:
            std::cerr << " error command" << std::endl;
            break;
        }
        response += '\n';
        // sending response
#ifdef DEBUG
        std::cout << " sending response: " << response << std::endl;
#endif
        boost::asio::write(socket, boost::asio::buffer(response));
    } catch (std::exception& e) {
        std::cerr << " error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    auto port = 8095;   // default port
    try{
        auto config_name = "config.txt";
        port = config_get_port(config_name);
    }
    catch(...){
        std::cerr << " failed opening config file; used default parameters" << std::endl;
    }

    //
    try {
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
#ifdef DEBUG
        std::cout << " start working" << std::endl;
#endif
        while (true) {
            boost::asio::ip::tcp::socket socket(ioContext);
            acceptor.accept(socket);
            
            // start client handler in new thread
            std::thread thread(clientHandler, std::move(socket));
            thread.detach();
        }
    } catch (std::exception& e) {
        std::cerr << " error: " << e.what() << std::endl;
    }
    return 0;
}
