#ifndef HUFFMAN_H
#define HUFFMAN_H

#include"node.h"
#include<queue>
#include<vector>
#include<map>
namespace Compress
{
class huffman
{
private:
    node* root;
public:
    huffman(){root = nullptr, nodeCount = 0;}
    huffman(node* r,int b){root = r;nodeCount = b;}
    int nodeCount;
    node* getRoot() const {return root;}
    bool isEmpty(){return root == nullptr;}
    void create(std::vector<node*>& byteVrctor);
    int getWPL(){return root->getVal();}
    std::map<char,std::string> getHuffmanCode();
    // ~huffman();    
    void dfsFree(node* cur);
};
}

#endif