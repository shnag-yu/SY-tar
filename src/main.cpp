#include "../include/huffman.h"
#include "../include/comp.h"
#include "../include/uncomp.h"
#include "../include/fileSys.h"
#include "../include/fileNode.h"
#include "../include/dirComp.h"
#include "../include/dirUncomp.h"
#include<queue>
#include <filesystem>
using namespace Compress;
namespace fs = std::filesystem;

int main(int argc,char *argv[])
{
    std::string command = argv[1];
    if(command == "comp")
    {
        if(argc != 4)
        {
            std::cout << "Wrong Command!" << std::endl;
            return 1;
        }
        std::string source = argv[2];
        std::string dest = argv[3];
        if(isInCurDir(dest))
            return 0;
        fs::path p = fs::current_path()/source;
        if(fs::is_directory(p))
        {
            fileNode* root = new fileNode(p);
            dirComp* dirComp_ = new dirComp(root, source, dest);
            dirComp_->recursion(dirComp_->getDirParh(), dirComp_->getRoot());
            dirComp_->close();
            delete dirComp_;
        }
        else{
            comp* comp_ = new comp(source, dest);
            comp_->work();
            delete comp_;
        }
        return 0;
    }

    if(command == "uncomp")
    {
        if(argc != 3)
        {
            std::cout << "Wrong Command!" << std::endl;
            return 1;
        }
        std::string source = argv[2];

        std::ifstream in(source, std::ios::in);
        std::string headTag;
        in >> headTag;
        in.close();
        if(headTag == "produced")
        {
            dirUncomp* dirUncomp_ = new dirUncomp(source);
            dirUncomp_->createFileTree(1);
            delete dirUncomp_;
        }
        else if(headTag == "Produced")
        {
            uncomp* uncomp_ = new uncomp(source);
            uncomp_->work();
            delete uncomp_;
        }
        else{
            std::cout << "NOT MY COMPRESS FILE!" <<std::endl;
        }
        return 0;
    }

    if(command == "display")
    {
        if(argc != 3)
        {
            std::cout << "Wrong Command!" << std::endl;
            return 1;
        }
        std::string source = argv[2];
        std::ifstream in(source, std::ios::in);
        std::string headTag;
        in >> headTag;
        in.close();
        if(headTag == "produced")
        {
            dirUncomp* dirUncomp_ = new dirUncomp(source);
            dirUncomp_->display();
            delete dirUncomp_;
        }
        else if(headTag == "Produced")
        {
            uncomp* uncomp_ = new uncomp(source);
            uncomp_->display();
            delete uncomp_;
        }
        else{
            std::cout << "NOT MY COMPRESS FILE!" <<std::endl;
        }
        return 0;
    }
    // dirUncomp* dirUncomp_ = new dirUncomp("test.comp");
    // dirUncomp_->createFileTree(1);
    // delete dirUncomp_;
    
}
