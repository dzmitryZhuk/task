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
    // string text = "this text";
    // addCommand(text, Count);
    // //
    // cout << "add command to text" << endl;
    // for(const auto& i : text)
    //     cout << "<" << static_cast<int>(i) << "> " << endl;
    // //
    // auto temp = separateStrCommand(text);
    // auto command = stringToCommand(temp);
    // //
    // cout << "text after separate" << endl;
    // for(const auto& i : text)
    //     cout << "<" << static_cast<int>(i) << "> " << endl;
    // //
    // //
    // cout << "command after separate" << endl << command << endl;
    // //
    return 0;
}

// // client
// int main(int argc, char* argv[])
// {
//     string s = "qqq";
//     Command c = Hash;
//     addCommand(s,c);
//     for(const auto& i : s)
//         cout << "<" << static_cast<int>(i) << "> " << endl;
//     // auto s = commandToString(Command::Hash);
//     return 0;
// }