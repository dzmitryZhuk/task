#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "common.h"
#include "server_functions.h"
#include "client_functions.h"
using namespace std;

// server
int main(int argc, char* argv[])
{
    std::cout << "TEST RUNNING" << std::endl;
    std::string s;
    if(argc == 2)
        s = argv[1];
    else
        s = "\ns\na";
    auto res = countHash(s);
    for(const auto& i : res)
        std::cout << "<" << (int)i << ">";
    std::cout << std::endl;
    return 0;
}
