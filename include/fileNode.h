#ifndef FILENODE_H
#define FILENODE_H

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>      
using namespace std::filesystem;

namespace Compress
{
class fileNode
{
public:
    fileNode(const path& newPath) 
    {
        filePath = newPath;
    }
    void addChild(fileNode* child)
    {
        children.push_back(child);
    }
private:
    path filePath;
    std::vector<fileNode*> children;
};
}

#endif