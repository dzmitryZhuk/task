#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <vector>
#include <array>

#include <boost/asio.hpp>

#include "common.h"

class Server{
    int m_port;

public: 
    Server(const int& port) : m_port(port) {}
    void run();
protected:
    size_t countWords(const std::string& text);
    std::vector<uint8_t> countHash(const std::string& text);
// -------------- functions for hash processing
    // function that xor two vectors and return result
    std::vector<uint8_t> xorVectors(const std::vector<uint8_t>& v1, const std::vector<uint8_t>& v2);
    // function multiply every fragment by the matrix L
    std::vector<uint8_t> matrixMultiply(std::vector<uint8_t>& vec, const std::array<std::array<int, 4>, 4>& L);
    // function that fills the vector <vec> with zeros to the end up to size <len>
    void fillZeros(std::vector<uint8_t>& vec, const size_t& length);
// --------------
    void clientHandler(boost::asio::ip::tcp::socket socket);
};

#endif