#include <iostream>
#include <sstream>
#include <thread>
#include <future>
#include <numeric>

#include "Server.h"

size_t Server::countWords(const std::string& text)
{
    std::stringstream temp(text);
    std::string word;
    size_t count = 0;

    while (temp >> word)
        count++;

    return count;
}

//-------------------------------------------------

std::array<std::array<int, 4>, 4> L = {{
    {{0x2, 0x3, 0x1, 0x1}},
    {{0x1, 0x2, 0x3, 0x1}},
    {{0x1, 0x1, 0x2, 0x3}},
    {{0x3, 0x1, 0x1, 0x2}}
}};

std::array Sbox = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

std::vector<uint8_t> Server::countHash(const std::string& _text){
    std::vector<uint8_t> res;
    if(_text.empty())
        return res;
    auto text = _text;
    auto lcm = std::lcm(Sbox.size(),L.size());
    // нужно дополнить исходную строку нулями. кол-во нулей = остаток от деления на НОК ? НОК - остаток от деления на НОК : 0
    fillZeros(text, text.size() % lcm ? lcm - text.size() % lcm : 0);
    // first 4 bytes
    auto _hash4 = [this, &res, &text]() {
        // separete on fragments 4 bytes
        std::vector<uint8_t> four;
        std::vector<std::vector<uint8_t>> fragments;
        std::string fragment;
        for (size_t i = 0; i < text.size(); i += 4) {
            fragment = text.substr(i, 4);
            std::vector<uint8_t> vec(fragment.begin(), fragment.end());
            fragments.push_back(vec);
        }

        for(auto& i : fragments)
            i = matrixMultiply(i, L);
        // xor all fragments
        four = fragments[0];
        for(int i = 1; i < fragments.size(); i++)
            four = xorVectors(four, fragments[i]);

        res.insert(res.end(), four.begin(), four.end());
    };
    
    auto _hash7 = [this, &res, &text]() {
        // second 7 bytes
        std::vector<uint8_t> seven;
        std::vector<std::vector<uint8_t>> fragments;
        // xor Sbox
        auto cur_Sbox = Sbox;
        for(int i = 0; i < cur_Sbox.size(); ++i)
            cur_Sbox[i] ^= text[0];

        // replace all characters by Sbox
        for (size_t i = 0; i < text.size(); i += 7) {
            std::vector<uint8_t> vec;
            for(int j = 0; j < 7; j++){
                if(i + j >= text.size())
                    break;
                vec.push_back(cur_Sbox[text[i + j]]);
            }
            fragments.push_back(vec);
        }

        // xor all fragments
        seven = fragments[0];
        for(int i = 1; i < fragments.size(); i++)
            seven = xorVectors(seven, fragments[i]);
        
        res.insert(res.end(), seven.begin(), seven.end());
    };

    std::future<void> hash4 = std::async(std::launch::async, _hash4);
    hash4.get();
    std::future<void> hash7 = std::async(std::launch::async, _hash7);
    hash7.get();

    return res;
}

std::vector<uint8_t> Server::xorVectors(const std::vector<uint8_t>& v1, const std::vector<uint8_t>& v2)
{
    if(v1.size() != v2.size()) return v1;
    std::vector<uint8_t> res;
    res.reserve(v1.size());
    for(int i = 0; i < v1.size(); ++i)
        res.push_back(v1[i] ^ v2[i]);
    return res;
};

std::vector<uint8_t> Server::matrixMultiply(std::vector<uint8_t>& vec, const std::array<std::array<int, 4>, 4>& L)
{
    if(!L.size() || vec.size() != L[0].size()) return vec;
    std::vector<uint8_t> res;
    for(int i = 0; i < vec.size(); ++i)
    {
        res.push_back(0);
        for(int j = 0; j < vec.size(); ++j)
            res[i] += vec[j] * L[j][i];
    }
    return res;
};

void Server::fillZeros(std::string& vec, const size_t& len)
{
    while(vec.size() < len)
        vec.push_back(0);
};

void Server::clientHandler(boost::asio::ip::tcp::socket socket) {
#ifdef DEBUG
    static int client_count = 0;
    client_count++;
    log(std::string(" client connection number ") + std::to_string(client_count));
#endif
    try {
        // reading client data
        boost::asio::streambuf buffer;

        // reading command
        auto bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(sizeof(Command)));
        if(bytes_received != sizeof(Command))
        {
            log(std::string(" failed receive command from client number ") + std::to_string(client_count));
            return;
        }
        auto command = *boost::asio::buffer_cast<const Command*>(buffer.data());
        buffer.consume(bytes_received);

        // reading file size
        bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(sizeof(filesize_t)));
        if(bytes_received != sizeof(filesize_t))
        {
            log(std::string(" failed receive file size from client number ") + std::to_string(client_count));
            return;
        }
        auto size = *boost::asio::buffer_cast<const filesize_t*>(buffer.data());
        buffer.consume(bytes_received);

        // read text
        std::string message;
        message.reserve(size);
        bytes_received = boost::asio::read(socket, buffer, boost::asio::transfer_exactly(size));
        if(bytes_received != size) 
        {
            log(std::string(" failed receive file content from client number ") + std::to_string(client_count));
            return;
        }
        const char* data = boost::asio::buffer_cast<const char*>(buffer.data());
        message.append(data, size);

        // make response
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
        log(std::string(" sending response to client number ") + std::to_string(client_count));
        if(command == Command::Count)
            std::cout << response;
        else
            for(const auto& i : response)
                std::cout << std::hex << (static_cast<int>(i) & 0xff);
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

void Server::run()
{
    try {
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::acceptor acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port));
        log(" start working");
        while (true) {
            boost::asio::ip::tcp::socket socket(ioContext);
            acceptor.accept(socket);
            
            // start client handler in new thread
            std::thread thread(&Server::clientHandler, this, std::move(socket));
            thread.detach();
        }
    } catch (const std::exception& e) {
        std::cerr << " error: " << e.what() << std::endl;
    }
    catch(const char* message){
        std::cerr << message << std::endl;
    }
}
