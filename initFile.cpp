#include<vector>
#include<fstream>
#include<iostream>


int main()
{
    std::cout << "input file size: \n";
    int fileSize;
    std::cin>> fileSize;
    std::vector<int> f = {356,139,279,378,1304,289,199,528,627,13,420,339,249,707,797,199,12,677,607,1045,249,92,149,17,199,8};
    std::ofstream init("test.txt",std::ios::out);
    for(int i = 0; i < f.size(); i++)
    {
        for(int j = 0; j < f[i]; j ++)
            for(int k = 0; k < fileSize; k++)
                init << (char)(i + 'a');
    }
    init.close();
    return 0;
}