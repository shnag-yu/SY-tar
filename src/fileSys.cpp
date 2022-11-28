#include "../include/fileSys.h"
namespace fs = std::filesystem;

int isInCurDir(std::string filename)
{
    //return 1 to STOP 
    //return 0 to CONTINUE
    fs::path s = fs::current_path()/filename;
    for(auto p : fs::directory_iterator(fs::current_path()))
    {
        if(s == p.path())
        {
            std::cout << s 
            << "  has existed!\nPress [C] to cover existed file; Press [S] to stop compressing.\n";
            char option = '\0';
            while(option != 'S' && option != 'C')
            {
                std::cin >> option;
                if(option == 'S')
                {
                    std::cout << "Compression has stopped." <<std::endl;
                    return 1;
                }
                else if (option == 'C')
                {
                    std::cout << "Compression continued." << std::endl;
                    return 0;
                }
                else 
                    std::cout << "Wrong Command\nPress [C] to cover existed file; Press [S] to stop compressing.\n";
            }
        }
    }
    return 0;
}