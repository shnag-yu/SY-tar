#ifndef COMP_H
#define COMP_H

#include "huffman.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Compress
{
class comp
{
private:
    std::filesystem::path path;
    std::string fileName;
    huffman huffmanTree;
    long long byteCount;
    std::map<char,std::string> huffmanCode;
    int byteType;
    std::vector<long long> freq;
    std::ifstream inFile;
    std::ofstream outFile;
public:
    comp();
    comp(std::string in, std::string out);
    comp(std::string in, std::string out,int);
    int readFile();
    int huffmanEncode();
    int freqCount(std::vector<int>& freq); 
    int writeHead();
    int writeBody();
    int work();    
};
}
#endif