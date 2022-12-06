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
        outFile.close();                  
        outFile.open(zipName, std::ios::binary | std::ios::app);
        outFile.seekp(0,std::ios::end);
        char c = (char)255;
        outFile.write((char*)&c,sizeof(char));
        outFile << tmp;
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
            curFile->work();
            delete curFile;
        }
    }
}
