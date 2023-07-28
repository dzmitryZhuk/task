#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

#include <boost/asio.hpp>

#include "common.h"

class Client{
    int m_port;
    std::string m_host;
    std::string m_fileText;
    std::filesystem::path m_path;
    boost::asio::ip::tcp::socket *m_socket;
    Command m_command;
public:
    Client(const std::string& host, const int& port) : m_port(port), m_host(host) {}
    ~Client() {
        delete m_socket;
    }
    bool connectToServer();
    bool readTextFromFile(const std::filesystem::path& file);
    bool sendText(const Command& command);
    std::string getResponse();
    void disconnectFromServer();
protected:
    filesize_t getFileSize(const std::filesystem::path& path);
};

#endif