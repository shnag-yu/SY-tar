#include "../include/uncomp.h"
#include "../include/fileSys.h"
#include <filesystem>
#include <windows.h>
#include <fstream>
using namespace Compress;
namespace fs = std::filesystem;

uncomp::uncomp(std::string in)
{
    fileName = in;
    byteCount = 0;
    byteType = 0;
    inFile = std::ifstream(in,std::ios::in | std::ios::binary);
    p = 0;
}

uncomp::uncomp(std::string in, std::streampos pos)
{
    fileName = in;
    byteCount = 0;
    byteType = 0;
    inFile = std::ifstream(in,std::ios::in | std::ios::binary);
    // std::cout << "pos: " << inFile.tellg() << std::endl;
    p = pos;
    inFile.seekg(pos);
    // std::cout << "pos: " << inFile.tellg() << std::endl;

}

int uncomp::readHead()
{
    if(!inFile)
    {
        std::cout << "error: failed to read file" << std::endl;
        return 1;
    }
    unsigned char c = '\0';
    // std::cout << "pos: " << inFile.tellg() << std::endl;
    while(inFile.read((char*)&c, sizeof(char)))
        byteCount ++;
    inFile.close();
    
    inFile.open(fileName, std::ios::in | std::ios::binary);
    if(p != 0)
        inFile.seekg(p);
    // std::cout << "pos: " << inFile.tellg() << std::endl;
    std::string tag;
    const std::string headTag = "Produced By SY Compress Program.";
    for(int i =0; i < headTag.length(); i++)
    {
        inFile.read((char*)&c, sizeof(char));
        tag.push_back(c);
    }
    // std::string Tag = tag;
    //std::clog << std::endl << Tag <<std::endl;

    if(tag != headTag)
    {
        std::cout << "This is NOT produced by my program!" << std::endl;
        inFile.close();
        return 1;
    }

    inFile.read((char*)&byteType, sizeof(byteType));
    // std::cout << "pos: " << inFile.tellg() << std::endl;

    //std::clog << byteCount <<std::endl;
    inFile.read((char*)&byteCount, sizeof(byteCount));
    // std::cout << "pos: " << inFile.tellg() << std::endl;

    int lenOfFileName;
    inFile.read((char*)&lenOfFileName, sizeof(lenOfFileName));
    char* filename = (char*)malloc((lenOfFileName + 1) * sizeof(char));
    inFile.read((char*)(filename), lenOfFileName);
    fileName = filename;
    free(filename);
    // std::cout <<fileName << std::endl;
    if(isInCurDir(fileName))
    {
        std::cout << "Uncompression stopped\n";
        return 1;
    }
    outFile = std::ofstream(fileName,std::ios::out|std::ios::binary);
    freq = std::vector<long long>(256,0);
    if(byteCount == 0)
    {
        inFile.close();
        return 1;
    }
    for(int i =0; i < byteType; i ++)
    {
        int j;
        inFile.read((char*)&j, sizeof(j));
        inFile.read((char*)&freq[j], sizeof(freq[j]));
    }
    // outFile = std::ofstream(fileName,std::ios::out|std::ios::binary);
    // outFile = std::ofstream("../out.png",std::ios::out|std::ios::binary);
    return 0;
}


int uncomp::huffmanEncode()
{
    std::vector<node*> byteVector;
    for(int i = 0; i < 256; i++)
    {
        if(freq[i] != 0)
        {
            node* tmp = new node(i, freq[i], nullptr, nullptr);
            byteVector.emplace_back(tmp);
        }
    }
        //std::cout << i->getIdx() << " " << i->getVal() <<std::endl;

    huffmanTree.create(byteVector);
    return 0;
}

int uncomp::write()
{
    unsigned char c;
    node* cur = huffmanTree.getRoot();
    //bool isLeaf = 0;
    unsigned char curByte = '\0';
    int curBitIndex = 0;
    int i = 0;
    long long pos = 0;
    while(inFile.read((char*)&curByte, sizeof(char)))
    {
        //std::cout << pos <<std::endl;
        while(curBitIndex != 8)
        {
            if(pos >= byteCount)
                break;
            int curBit = ( curByte >> (7 - curBitIndex) ) &  1 ;
            curBitIndex ++;
            if(curBit == 0)
            {
                cur = cur->getLeft();
            }
            else if(curBit == 1)
            {
                cur = cur->getRight();
            }
            if(cur->isLeaf())
            {
                char tmp = cur->getIdx();
                outFile.write((char*)&tmp, sizeof(tmp));
                pos ++;
                //std::cout << tmp;
                cur = huffmanTree.getRoot();
            }
        }
        if(curBitIndex == 8)
        {
            curBitIndex = 0;
            //curByte = '\0';
        }
    }
    outFile.close();
    return 0;
}

int uncomp::work()
{
    if(!this->readHead())
        if(!this->huffmanEncode())
            this->write();
    // fs::path p = ".";
    // fs::rename(p/"tmp", fileName);
    return 0;
}

int uncomp::display()
{
    if(!inFile)
    {
        std::cout << "error: failed to read file" << std::endl;
        return 1;
    }
    unsigned char c = '\0';
    std::string tag;
    const std::string headTag = "Produced By SY Compress Program.";
    for(int i =0; i < headTag.length(); i++)
    {
        inFile.read((char*)&c, sizeof(char));
        tag.push_back(c);
    }
    if(tag != headTag)
    {
        std::cout << "This is NOT produced by my program!" << std::endl;
        inFile.close();
        return 1;
    }

    inFile.read((char*)&byteType, sizeof(byteType));
    inFile.read((char*)&byteCount, sizeof(byteCount));
    int lenOfFileName;
    inFile.read((char*)&lenOfFileName, sizeof(lenOfFileName));
    char* filename = (char*)malloc((lenOfFileName + 1) * sizeof(char));
    inFile.read((char*)(filename), lenOfFileName);
    fileName = filename;
    free(filename);
    std::cout << fileName <<std::endl;
    return 0;
}

