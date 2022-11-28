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
    // std::cout << "beg: " << std::hex <<outFile.tellp()<<std::endl;
    // long long pos = outFile.tellp();
    // outFile.write((char*)&pos, sizeof(pos));
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
    // bool bigFile = byteCount > 1 << 30;
    //std::cout << byteCount << std::endl;
    // long long readPos = 0;
    // std::vector<unsigned char> fileData(1<<21);
    freq = std::vector<long long>(256);
    inFile.open(path, std::ios::in | std::ios::binary | std::ios::app);
    while(inFile.read((char*)&c, sizeof(char)))
    {
        freq[(int)c]++;
        // std::cout<<c;
    }
    inFile.close();
    for(int i = 0; i < 256; i++)
    {
        if(freq[i] != 0)
            byteType++;
    }
    // std::cout << byteType << std::endl;
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

    // for(auto i : byteVector)
    //     std::cout << i->getIdx() << " " << i->getVal() <<std::endl;

    huffmanTree.create(byteVector);

    std::map<char,std::string> huffmanCode;
    huffmanCode = huffmanTree.getHuffmanCode();
    //std::cout <<huffmanCode.size() <<std::endl;
    return 0;
}

int comp::writeHead()
{   
    // std::cout << "pos: " << outFile.tellp() << std::endl;
    //Write the head infomation (include byte frequencies) into file.
    std::string headTag = "Produced By SY Compress Program.";
    // std::cout << outFile.tellp() << std::endl;
    // outFile.seekp(std::ios::end);
    // std::cout << outFile.tellp() << std::endl;
    //std::cout<< "headTag Size = " << sizeof(headTag) << std::endl;
    outFile.write((char*)headTag.c_str(), sizeof(headTag));

    outFile.write((char*)&byteType, sizeof(byteType));
    outFile.write((char*)&byteCount, sizeof(byteCount));

    int lenOfFileName = sizeof(fileName);
    outFile.write((char*)&lenOfFileName, sizeof(lenOfFileName));
    outFile.write((char*)fileName.c_str(), sizeof(fileName));

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
    //The head info should occupy sizeof(headTag) + sizeof(byteType) + sizeof(byteCount) + sizeof(lenOfFileName) + sizeof(fileName) + byteType*5
    // std::cout << outFile.tellp() << std::endl;
    return 0;
}

int comp::writeBody()
{
    // std::cout << outFile.tellp() << std::endl;
    if(byteCount == 0)
        return 0;
    //std::cout << std::endl;
     //Write the huffmanCodes into file.
    char byte = '\0';
    //std::cout << std::hex << (int)byte <<std::endl;
    bool isByteEmpty = true;
    int bitIndex = 0;
    if(huffmanTree.getRoot() == nullptr)
        return 1;
    huffmanCode = huffmanTree.getHuffmanCode();
    //std::cout << huffmanCode.size() <<std::endl;
    inFile.open(path, std::ios::in | std::ios::binary);
    for (int i = 0; i < byteCount; i++)
    {
        int codeIndex = 0;
        char tempChar = '\0';
        //cout << (int)fileData[i] << endl;
        unsigned char cc;    
        inFile.read((char*)&cc, sizeof(char));
        std::string code = huffmanCode[cc];
        while (codeIndex <  code.length())
        {
            char bit = code[codeIndex] - '0';
            // printf("%x %x\n",bit,byte);
            byte |= (bit <<  (7 - bitIndex) );
            bitIndex ++;
            codeIndex ++;
            isByteEmpty = false;
            if (bitIndex == 8)
            {
                //cout << "!" << (int)(unsigned char)byte << endl;
                outFile.write((char*)&byte, sizeof(byte));
                // std::cout << (int)byte;
                // std::cout << byte;
                // printf("%x\n",byte);
                byte = '\0';
                isByteEmpty = true;
                bitIndex = 0;
            }
        }
    }
 
 
    if(!isByteEmpty) 
    {
        outFile.write((char*)&byte, sizeof(byte));
        // printf("%x\n",byte);
    }
    // outFile.seekp(outFile.tellp());
    // std::cout << outFile.tellp() << std::endl;
    inFile.close();
    // std::cout << outFile.tellp() <<std::endl;

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
