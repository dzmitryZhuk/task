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
    if(ec)
        log_error(" failed connecting to host " + host + " via port " + std::to_string(port));
    else
        log(" connected to server " + host + " on port " + std::to_string(port));

    return m_socket->is_open();
}

bool Client::readTextFromFile(const std::filesystem::path& path)
{
    std::ifstream file(path);
    if (file.is_open()) {
        auto size = std::filesystem::file_size(path);
        m_text.resize(size);
        file.read(m_text.data(), size);
        file.close();
    } else {
        log_error(" failed read text from file " + path.string());
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
        log_error(" failed sending command");
        return false;
    }
    log(std::string(" command <") + std::to_string(static_cast<int>(command)) + std::string("> sent"));
    // file size sending
    filesize_t size = m_text.size();
    boost::asio::write(*m_socket, boost::asio::buffer(&size, sizeof(decltype(size))), ec);
    if(ec){
        log_error(" failed sending file size");
        return false;
    }
    log(std::string(" size <") + std::to_string(size) + std::string("> sent"));
    // message sending
    boost::asio::write(*m_socket, boost::asio::buffer(m_text), ec);
    if(ec){
        log_error(" failed sending text");
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
        log_error(" failed receive response from server");
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
