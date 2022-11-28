#include "../include/fileNode.h"
#include "../include/dirComp.h"
#include "../include/comp.h"

using namespace Compress;

dirComp::dirComp(fileNode* rt, std::string dir, std::string out)
{
    root = rt;
    zipName = out;
    dirPath = current_path()/dir;
    outFile = std::ofstream(out, std::ios::out |std::ios::binary);
    outFile << "produced By SY Compress Program.";
    outFile << relative(dir, current_path()) << "\n";                                    
    // std::cout << relative(dir, current_path()) << "\n";
}

void dirComp::recursion(path src_path, fileNode& parent)
{
    directory_iterator end;                       
    directory_iterator dir(src_path);                    
    
    for(dir; dir != end; dir++)
    {
        fileNode* newNode = new fileNode(dir->path());   
        parent.addChild(newNode);  
        auto tmp = relative(dir->path(), current_path());
        // std::cout << tmp << "\n";    
        outFile.close();                  
        outFile.open(zipName, std::ios::binary | std::ios::app);
        // std::cout << outFile.tellp() <<std::endl;
        outFile.seekp(0,std::ios::end);
        // std::cout << outFile.tellp() <<std::endl;
        char c = (char)255;
        outFile.write((char*)&c,sizeof(char));
        // std::cout << outFile.tellp() <<std::endl;
        outFile << tmp ;
        // std::cout << outFile.tellp() <<std::endl;
        outFile.close(); 
        if(is_directory(*dir)) 
            recursion(*dir, *newNode);
        else{
            outFile.open(zipName, std::ios::binary | std::ios::app);
            outFile.seekp(0,std::ios::end);
            char c = (char)254;
            outFile.write((char*)&c,sizeof(char));
            outFile.write((char*)&c,sizeof(char));
            outFile.close(); 
            comp* curFile = new comp(relative(dir->path(), current_path()).string(), zipName, 0);
            // std::cout << "filename: " << relative(dir->path(), current_path()).string() << std::endl;
            curFile->work();
            delete curFile;
        }
    }
}
