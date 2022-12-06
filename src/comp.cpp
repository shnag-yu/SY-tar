#include "../include/comp.h"
#include <map>
#include <iomanip>

using namespace Compress;

comp::comp(std::string in, std::string out)
{
    path = in;
    fileName = path.filename().string();
    byteCount = 0;
    byteType = 0;
    inFile = std::ifstream(in,std::ios::in | std::ios::binary);
    outFile = std::ofstream(out,std::ios::out|std::ios::binary);
}

comp::comp(std::string in, std::string out, int x)
{
    path = in;
    fileName = path.string();
    byteCount = 0;
    byteType = 0;
    inFile = std::ifstream(in,std::ios::in | std::ios::binary);
    outFile = std::ofstream(out,std::ios::out|std::ios::binary| std::ios::app);
    outFile.seekp(0, std::ios::end);
}

int comp::readFile()
{
    if(!inFile)
    {
        std::cout << "error: failed to read file" << std::endl;
        return 1;
    }
    unsigned char c = '\0';
    while(inFile.read((char*)&c, sizeof(char)))
         byteCount ++;
    inFile.close();
    if(byteCount == 0)
    {
        return 0;
    }
    freq = std::vector<long long>(256);
    inFile.open(path, std::ios::in | std::ios::binary | std::ios::app);
    while(inFile.read((char*)&c, sizeof(char)))
    {
        freq[(int)c]++;
    }
    inFile.close();
    for(int i = 0; i < 256; i++)
    {
        if(freq[i] != 0)
            byteType++;
    }
    return 0;
}

int comp::huffmanEncode()
{
    //create huffmanTree
    std::vector<node*> byteVector;
    for(int i = 0; i < 256; i++)
    {
        if(freq[i] != 0)
        {
            byteType ++;
            node* tmp = new node(i, freq[i], nullptr, nullptr);
            byteVector.push_back(tmp);
        }
    }

    huffmanTree.create(byteVector);

    std::map<char,std::string> huffmanCode;
    huffmanCode = huffmanTree.getHuffmanCode();
    return 0;
}

int comp::writeHead()
{   
    //Write the head infomation (include byte frequencies) into file.
    std::string headTag = "Produced By SY Compress Program.";
    outFile.write((char*)headTag.c_str(), sizeof(headTag));

    outFile.write((char*)&byteType, sizeof(byteType));
    outFile.write((char*)&byteCount, sizeof(byteCount));

    outFile << fileName << '\n';
    if(byteCount == 0)
        return 1;

    for(int i = 0; i < 256; i ++)
    {
        if(freq[i])
        {
            outFile.write((char*)&i, sizeof(i));
            outFile.write((char*)&freq[i], sizeof(freq[i]));
        }
    }
    return 0;
}

int comp::writeBody()
{
    if(byteCount == 0)
        return 0;
    char byte = '\0';
    bool isByteEmpty = true;
    int bitIndex = 0;
    if(huffmanTree.getRoot() == nullptr)
        return 1;
    huffmanCode = huffmanTree.getHuffmanCode();
    inFile.open(path, std::ios::in | std::ios::binary);
    for (int i = 0; i < byteCount; i++)
    {
        int codeIndex = 0;
        char tempChar = '\0';
        unsigned char cc;    
        inFile.read((char*)&cc, sizeof(char));
        std::string code = huffmanCode[cc];
        while (codeIndex <  code.length())
        {
            char bit = code[codeIndex] - '0';
            byte |= (bit <<  (7 - bitIndex) );
            bitIndex ++;
            codeIndex ++;
            isByteEmpty = false;
            if (bitIndex == 8)
            {
                outFile.write((char*)&byte, sizeof(byte));
                byte = '\0';
                isByteEmpty = true;
                bitIndex = 0;
            }
        }
    }
 
 
    if(!isByteEmpty) 
    {
        outFile.write((char*)&byte, sizeof(byte));
    }
    inFile.close();

    outFile.close();
    return 0;
}

int comp::work()
{
    if(!this->readFile())
    {
        if(!this->writeHead())
        {
            if(!this->huffmanEncode())
                this->writeBody();
        }
    }
    return 0;
}
