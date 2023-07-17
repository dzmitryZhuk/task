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
    log(std::string(" client connection №") + std::to_string(client_count));
#endif
    try {
        // reading client data
        boost::asio::streambuf buffer;

        // reading command
        auto bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(sizeof(Command)));
        if(bytes_received != sizeof(Command))
            throw 1;

        const Command* command_ptr = boost::asio::buffer_cast<const Command*>(buffer.data());
        auto command = *command_ptr;
        //command = stringToCommand(make_string(buffer));
        buffer.consume(bytes_received);

        // reading file size
        filesize_t file_size;
        bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(sizeof(filesize_t)));
        if(bytes_received != sizeof(filesize_t))
            throw 1;

        const filesize_t* size_ptr = boost::asio::buffer_cast<const filesize_t*>(buffer.data());
        auto size = *size_ptr;
        buffer.consume(bytes_received);

        // read text
        std::string message;
        message.reserve(size);
        bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(size));
        if(bytes_received != size) 
            throw 1;
        const char* data = boost::asio::buffer_cast<const char*>(buffer.data());
        message.append(data, size);

        std::string response;
        switch (command)
        {
        case Command::Count:{
            response = std::to_string(countWords(message));
            break;
        }
        case Command::Hash:{
            auto temp = countHash(message);
            response = std::string(temp.begin(), temp.end());
            break;
        }
        case Command::No_command:
        default:
            std::cerr << " error command" << std::endl;
            break;
        }
        response += '\n';
        // sending response
#ifdef DEBUG
        log(std::string(" sending response to client №") + std::to_string(client_count));
        if(command == Command::Count)
            std::cout << response;
        else
            for(const auto& i : response)
                std::cout << std::hex << static_cast<int>(i);
        std::cout << std::endl;
#endif
        boost::asio::write(socket, boost::asio::buffer(response));
    } catch (const std::exception& e) {
        std::cerr << " error: " << e.what() << std::endl;
    }
    catch(const char* message){
        std::cerr << message << std::endl;
    }
}

int main(int argc, char* argv[])
{
    auto cfg = readConfig("config.txt");

    try {
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), cfg.port));
        log(" start working");
        while (true) {
            boost::asio::ip::tcp::socket socket(ioContext);
            acceptor.accept(socket);
            
            // start client handler in new thread
            std::thread thread(clientHandler, std::move(socket));
            thread.detach();
        }
    } catch (const std::exception& e) {
        std::cerr << " error: " << e.what() << std::endl;
    }
    catch(const char* message){
        std::cerr << message << std::endl;
    }
    return 0;
}
