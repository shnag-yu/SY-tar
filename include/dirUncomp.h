#ifndef DIRUNCOMP_H
#define DIRUNCOMP_H

#include "fileNode.h"
#include "fileSys.h"

namespace Compress
{
struct entry
{
    int type;//1 for dir, 0 for file
    path p;
    int blank;
    entry(int t, path pa, int b)
    {
        type = t;
        p = pa;
        blank = b;
    }
};
class dirUncomp
{
private:
    fileNode* root;
    path dirPath;
    std::string zipName;
    std::ifstream inFile;
    std::ofstream outFile;
public:
    std::vector<entry> Entry;
    dirUncomp(std::string);
    fileNode& getRoot(){return *root;}
    int createFileTree(int);//1 for uncomp, 0 for Traversal           
    int display();
};
}

#endif