#include <fstream>
#include <filesystem>
#include <iostream>

#include "Client.h"

bool Client::connect(const std::string& host, const int& port)
{
    boost::asio::io_context ioContext;
    m_socket = new boost::asio::ip::tcp::socket(ioContext);
    boost::system::error_code ec;

    boost::asio::ip::tcp::resolver resolver(ioContext);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
    boost::asio::connect(*m_socket, endpoints, ec);
    if(ec){
        std::cerr << " failed connecting to host " << host << " via port " << port << std::endl;
        return false;
    }
    log(" connected to server");

    return m_socket->is_open();
}

bool Client::readTextFromFile(const std::filesystem::path& path)
{
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)){
            line.push_back('\n');
            m_text += line;
        }
        file.close();
    } else {
        std::cerr << " failed read text from file " + path.string() << std::endl;
        return false;
    }

    return true; 
}

bool Client::sendText(const Command& command)
{
    boost::system::error_code ec;
    // command sending
    boost::asio::write(*m_socket, boost::asio::buffer(&command, sizeof(decltype(command))), ec);
    if(ec){
        std::cerr << " failed sending command" << std::endl;
        return false;
    }
    log(std::string(" command <") + std::to_string(static_cast<int>(command)) + std::string("> sent"));
    // file size sending
    filesize_t size = m_text.size();
    boost::asio::write(*m_socket, boost::asio::buffer(&size, sizeof(decltype(size))), ec);
    if(ec){
        std::cerr << " failed sending file size" << std::endl;
        return false;
    }
    log(std::string(" size <") + std::to_string(size) + std::string("> sent"));
    // message sending
    boost::asio::write(*m_socket, boost::asio::buffer(m_text), ec);
    if(ec){
        std::cerr << " failed sending text" << std::endl;
        return false;
    }
    log(" data sent");

    return true;
}

std::string Client::getResponse()
{
    std::string res;
    boost::asio::streambuf buffer;
    boost::system::error_code ec;
    boost::asio::read_until(*m_socket, buffer, '\n', ec);
    if(ec){
        std::cerr << " failed receive response from server" << std::endl;
        return res;
    }
    log(" response received");
    std::istream input(&buffer);
    std::getline(input, res);
    return res;
}

void Client::disconnect()
{
    m_socket->close();
}

bool Client::sendText(const std::filesystem::path& file, const Command& command)
{
    if(!readTextFromFile(file))
        return false;
    if(!sendText(command))
        return false;
    return true;
}
