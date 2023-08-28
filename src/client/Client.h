#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

#include <boost/asio.hpp>

#include "common.h"

class Client{
    std::string m_text;
    boost::asio::ip::tcp::socket *m_socket;
    Command m_command;
public:
    Client() {
        m_socket = nullptr;
    }
    ~Client() {
        if(m_socket != nullptr)
            delete m_socket;
    }
    bool connect(const std::string& host, const int& port);
    std::string getResponse();
    void disconnect();
    // read text from file first, then send it with command
    bool sendText(const std::filesystem::path& file, const Command& command);

protected:
    // reads text from file to m_text
    bool readTextFromFile(const std::filesystem::path& file);
    bool sendText(const Command& command);
};

#endif