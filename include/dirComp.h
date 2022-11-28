#ifndef DIRCOMP_H
#define DIRCOMP_H

#include "fileNode.h"
using namespace std::filesystem;

namespace Compress
{
class dirComp
{
private:
    fileNode* root;
    std::string zipName;
    path dirPath;
    std::ofstream outFile;
public:
    dirComp(fileNode*, std::string, std::string);
    void recursion(path, fileNode&);
    fileNode& getRoot(){return *root;}
    path& getDirParh(){return dirPath;}
    void close(){this->outFile.close();}
};
}
#endif