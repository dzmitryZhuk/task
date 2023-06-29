#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <vector>

#include "common.h"

// Command stringToCommand(const std::string& str);    // converts the bytes of char to Command
// Command ptrToCommand(void * ptr); // creating command from data under <ptr>
// std::string separateStrCommand(std::string& text);  // separate command(bytes of char) from text and returns command
int countWords(const std::string& text);
std::vector<unsigned char> countHash(const std::string& text);

// function for hash processing

// function that xor two vectors and return result
auto xorVectors(const std::vector<unsigned char>& v1, const std::vector<unsigned char>& v2) -> std::vector<unsigned char>;
// function multiply every fragment by the matrix L
auto matrixMultiply(std::vector<unsigned char>& vec, const std::vector<std::vector<int>> L) -> std::vector<unsigned char>;
// function that fills the vector <vec> with zeros to the end up to size <len>
void fillZeros(std::vector<unsigned char>& vec, size_t len);

//

#endif