#include"../include/huffman.h"
#include<algorithm>
#include <functional>
#include<iostream>

using namespace Compress;

bool cmp(node* a, node* b)
{
    return a->getVal() > b->getVal();
}

void huffman::create(std::vector<node*>& byteVector)
{
    if(byteVector.empty())
    {
        std::cout << "Warning: You are trying to compress a empty file!" <<std::endl;
        return;
    }
    
    std::priority_queue<node*, std::vector<node*>, decltype(&cmp)> heap(cmp);
    for(auto i :byteVector)
        heap.push(i);
    nodeCount = heap.size();
    while(heap.size() >= 2)
    {
        node* newLeft = heap.top();
        heap.pop();
        node* newRight = heap.top();
        heap.pop();
        node* newFather = new node(-1, newLeft->getVal() + newRight->getVal(), newLeft, newRight);
        heap.push(newFather);
        //delete newFather;
    }
    root = heap.top();
    //std::cout << "WPL = " << root->getVal() << std::endl;
}


std::map<char,std::string> huffman::getHuffmanCode()
{
    std::queue<node*> Queue;
    Queue.push(root);
    std::map<char,std::string> huffmanCode;

    while(!Queue.empty())
    {  
        node* cur = Queue.front();
        Queue.pop();
                 
        if(cur->getLeft() != nullptr)
        {
            cur->getLeft()->code = cur->code + '0';
            Queue.push(cur->getLeft());
        }
        if(cur->getRight() != nullptr)
        {
            cur->getRight()->code = cur->code + '1';
            Queue.push(cur->getRight());
        }
        if(cur->isLeaf())
        {
            // std::cout << (int)cur->getIdx() << ": " << cur->getVal() << " " 
            //         << cur->code << std::endl;
            
            huffmanCode[cur->getIdx()] = cur->code;
        }
    }
    return huffmanCode;
}