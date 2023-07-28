#include <fstream>
#include <filesystem>

#include "Client.h"

filesize_t Client::getFileSize(const std::filesystem::path& path)
{
    // c++17 required
    return std::filesystem::file_size(path);
}

bool Client::connectToServer()
{
    boost::asio::io_context ioContext;
    m_socket = new boost::asio::ip::tcp::socket(ioContext);

    boost::asio::ip::tcp::resolver resolver(ioContext);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(m_host, std::to_string(m_port));
    boost::asio::connect(*m_socket, endpoints);
    log(" connected to server");

    return m_socket->is_open();
}

bool Client::readTextFromFile(const std::filesystem::path& path)
{
    m_path = path;

    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)){
            line.push_back('\n');
            m_fileText += line;
        }
        file.close();
    } else
        return false;

    return true; 
}

bool Client::sendText(const Command& command)
{
    // command sending
    boost::asio::write(*m_socket, boost::asio::buffer(&command, sizeof(decltype(command))));
    log(std::string(" command <") + std::to_string(static_cast<int>(command)) + std::string("> sent"));
    // file size sending
    auto size = getFileSize(m_path);
    boost::asio::write(*m_socket, boost::asio::buffer(&size, sizeof(decltype(size))));
    log(std::string(" size <") + std::to_string(size) + std::string("> sent"));
    // message sending
    boost::asio::write(*m_socket, boost::asio::buffer(m_fileText));
    log(" data sent");

    return true;
}

std::string Client::getResponse()
{
    // getting response
    boost::asio::streambuf buffer;
    boost::asio::read_until(*m_socket, buffer, '\n');
    log(" response received");
    std::istream input(&buffer);
    std::string response;
    std::getline(input, response);
    return response;
}

void Client::disconnectFromServer()
{
    m_socket->close();
}
