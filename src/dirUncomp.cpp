#include "../include/dirUncomp.h"
#include "uncomp.h"
#include <stack>

using namespace Compress;
namespace fs = std::filesystem;

dirUncomp::dirUncomp(std::string in)
{
    zipName = in;
    inFile = std::ifstream(in, std::ios::in | std::ios::binary);
}

int dirUncomp::createFileTree(int mode)
{
    if(!inFile)
    {
        std::cout << "error: failed to read file" << std::endl;
        return 1;
    }
    unsigned char c = '\0';
    inFile.close();
    inFile.open(zipName, std::ios::in | std::ios::binary);
    std::string tag;
    const std::string headTag = "produced By SY Compress Program.";
    for(int i = 0; i < headTag.length(); i++)
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
    fs::path p, rt;
    inFile >> rt;
    if(mode == 1)
        if(isInCurDir(rt.string()))
            return 0;
    if(mode == 1)   
    {   
        fs::remove_all(rt);
        fs::create_directory(current_path()/rt);
    }
    Entry.emplace_back(1, rt, 0);
    char cc;
    while(inFile.read((char*)&cc, sizeof(char)))
    {
        if(cc==(char)255)
        {
            inFile.read((char*)&cc, sizeof(char));
            inFile.seekg(-1, std::ios::cur);
            if(cc != '\"')
                continue;
            auto tmp = inFile.tellg();
            inFile >> p;
            if(p.string().find(rt.string()+"\\") == 0)
            {
                char ccc;
                inFile.read((char*)&cc, sizeof(char));
                inFile.read((char*)&ccc, sizeof(char));
                if(cc == (char)254 && ccc == (char)254)
                {
                    if(mode == 0)
                        Entry.emplace_back(0, p, 0);
                    if(mode == 1)
                    {
                        uncomp* uncomp_ = new uncomp(zipName, inFile.tellg());
                        uncomp_->work();
                        delete uncomp_;
                        // inFile.seekg(uncomp_->getP());
                    }
                }
                else{
                    inFile.seekg(-2, std::ios::cur);
                    if(mode == 0)
                        Entry.emplace_back(1, p, 0);
                    if(mode == 1)
                        fs::create_directory(current_path()/p);
                }
            }
            else
                inFile.seekg(tmp);
        }
    }
    return 0;
}

int getPre(path pre, path p)
{
    std::string p1 = pre.string();
    std::string p2 = p.string();
    if(p2.find(p1) < p2.size())
        return 1;
    return 0;
}

int dirUncomp::display()
{
    this->createFileTree(0);
    std::stack<entry> s;
    s.push(Entry[0]);
    std::cout << "<" << Entry[0].p.filename().string() << ">" <<std::endl;
    for(auto i = Entry.begin()+1; i != Entry.end(); i++)
    {
        auto pre = s.top();
        while(!s.empty())
        {
            pre = s.top();
            if(getPre(pre.p, (*i).p))
            {
                (*i).blank = pre.blank + 4;
                s.push(*i);
                break;
            }
            else{
                s.pop();
            }
        }
        if((*i).type == 1)
        {
            for(int j = 0; j <  pre.blank+3*(s.size()-1); j++)
                std::cout << " ";
            std::cout << "|";
            for(int j = 0; j < i->blank - pre.blank; j++)
                std::cout << "-";
            std::cout << "<" <<(*i).p.filename().string() << ">" <<std::endl;
        }
        else{
            for(int j = 0; j <  pre.blank+3*(s.size()-1); j++)
                std::cout << " ";
            std::cout << "|";
            for(int j = 0; j < i->blank - pre.blank; j++)
                std::cout << "-";
            std::cout << (*i).p.filename().string() <<std::endl;
        }
    }
    return 0;
}