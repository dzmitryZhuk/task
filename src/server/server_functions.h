#ifndef _SERVER_H_
#define _SERVER_H_

#include <string>
#include <vector>
#include <array>

#include "common.h"

size_t countWords(const std::string& text);
std::vector<unsigned char> countHash(const std::string& text);

// -------------- functions for hash processing

// function that xor two vectors and return result
std::vector<unsigned char> xorVectors(const std::vector<unsigned char>& v1, const std::vector<unsigned char>& v2);
// function multiply every fragment by the matrix L
std::vector<unsigned char> matrixMultiply(std::vector<unsigned char>& vec, const std::array<std::array<int, 4>, 4>& L);
// function that fills the vector <vec> with zeros to the end up to size <len>
void fillZeros(std::vector<unsigned char>& vec, const size_t& len);

// --------------

#endif