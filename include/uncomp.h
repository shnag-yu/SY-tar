#ifndef UNCOMP_H
#define UNCOMP_H

#include "huffman.h"
#include <iostream>
#include <fstream>

namespace Compress
{
class uncomp
{
private:
    std::string zipName;
    std::string fileName;
    huffman huffmanTree;
    long long byteCount;
    std::map<char,char> huffmanCode;
    int byteType;
    std::vector<long long> freq;
    std::ifstream inFile;
    std::ofstream outFile;
    std::streampos p;
public:
    uncomp(std::string,std::streampos);
    uncomp(std::string in);
    int readHead();
    int readFile();
    int huffmanEncode(); 
    int write();
    int work();
    int display();     
};
}

#endif