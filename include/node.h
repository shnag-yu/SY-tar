#ifndef NODE_H
#define NODE_H

#include<algorithm>
#include<string>
namespace Compress
{
class node 
{
private:
    char idx;
    int val;
    node* left;
    node* right;
public:
    node();
    node(int i,int v, node* l, node* r){idx = i; val = v; left = l; right =r;}
    std::string code;
    int getVal(){return val;}
    char getIdx(){return idx;}
    node* getLeft(){return left;}
    node* getRight(){return right;}
    bool isLeaf(){return left==nullptr && right == nullptr;}
};
}


#endif