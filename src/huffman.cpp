#include"../include/huffman.h"
#include<algorithm>
#include <functional>
#include<iostream>

using namespace Compress;

void huffman::dfsFree(node* cur)
{
    if(cur != nullptr)
    {
        if(cur->getLeft() != nullptr)
        {
            dfsFree(cur->getLeft());
            cur->left  = nullptr;
        }
        if(cur->getRight() != nullptr)
        {
            dfsFree(cur->getRight());
            cur->right = nullptr;
        }
        delete cur;
        cur = nullptr;
    } 
}


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
    }
    root = heap.top();
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
            
            huffmanCode[cur->getIdx()] = cur->code;
        }
    }
    return huffmanCode;
}