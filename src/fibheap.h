#ifndef FIBHEAP_H_
#define FIBHEAP_H_

#include <vector>
#include <cmath>
#include "arc.h"

using namespace std;
const int MAX_SIZE = 200000;


enum State {
    LABELED, UNLABELED, SCANNED
};

class Node_f {
public:
    Node_f * parent;
    Node_f * leftSibling, * rightSibling;
    Node_f * children;

    Node_f(int id, int key, struct arc * first);
    Node_f();

    struct arc *first; /* first arc in linked list */
    int id; /* Node_f id */
    int key;  /* Distance estimate, named key to reuse heap code*/
    int rank;
    int P; /* Predecessor node in shortest path */

    State state;

    bool addChild(Node_f * node);
    bool addSibling(Node_f * node);
    bool remove();

};


class FibHeap {
private:
    Node_f * rootListByRank[MAX_SIZE]{};
    Node_f * minRoot;
    Node_f * leftMostRoot;
    int numNodes;
    bool link(Node_f * root);

public:
    FibHeap();
    FibHeap(Node_f * root);
    ~FibHeap();
    bool isEmpty();
    void decreaseKey(Node_f * node, int newkey);
    bool insert(Node_f * node);
    Node_f * findMin();
    Node_f * removeMin();
};

Node_f::Node_f()
{
    this->parent = nullptr;
    this->children = nullptr;
    this->leftSibling = nullptr;
    this->rightSibling = nullptr;
    this->rank = 0;
    this->state = UNLABELED;
}

Node_f::Node_f(int id, int key, struct arc * first)
{
    this->id = id;
    this->key = key;
    this->first = first;
    this->parent = nullptr;
    this->children = nullptr;
    this->leftSibling = nullptr;
    this->rightSibling = nullptr;
    this->rank = 0;
    this->state = UNLABELED;
}

bool Node_f::addChild(Node_f * node)
{
    if (children != nullptr)
        children->addSibling(node);
    else
    {
        children = node;
        node->parent = this;
        rank = 1;        
    }

    return true;
}

bool Node_f::addSibling(Node_f * node)
{
    Node_f * tmp = this->rightSibling;
    this->rightSibling = node;
    node->leftSibling = this;
    node->rightSibling = tmp;
    node->parent = this->parent;
    if (tmp)
        tmp->leftSibling = node;

    if (this->parent)
        this->parent->rank++;
    return true;
}

bool Node_f::remove()
{
    if (parent)
    {
        parent->rank--;
        if (leftSibling)
            ;
        else if (rightSibling)
            parent->children = rightSibling;
        else 
            parent->children = nullptr;
    }
    if (leftSibling)
        leftSibling->rightSibling = rightSibling;
    if (rightSibling)
        rightSibling->leftSibling = leftSibling;
    
    rightSibling = leftSibling = nullptr;
    parent = nullptr;
    return true;
}


FibHeap::FibHeap()
{
    minRoot = nullptr;
    leftMostRoot = nullptr;
    numNodes = 0;
}

FibHeap::~FibHeap() = default;

bool FibHeap::isEmpty()
{
    return (minRoot == nullptr);
}

bool FibHeap::insert(Node_f * node)
{
    if (node == nullptr)
        return false;
    if (minRoot == nullptr)
    {
        minRoot = node;
        leftMostRoot = node;
    }
    else
    {
        minRoot->addSibling(node);
        if (minRoot->key > node->key)
            minRoot = node;
    }
    numNodes += 1;
    return true;
}

Node_f * FibHeap::findMin()
{
    return minRoot;
}

Node_f * FibHeap::removeMin()
{
    Node_f * tmp = minRoot->children;
    Node_f * nextTmp = nullptr;
    

    while (tmp != nullptr)
    {
        nextTmp = tmp->rightSibling;
        tmp->remove();
        minRoot->addSibling(tmp);
        tmp = nextTmp;
    }

    tmp = leftMostRoot;
    if (tmp == minRoot)
    {
        if (minRoot->rightSibling)
        {
            tmp = minRoot->rightSibling;
            leftMostRoot = tmp;
        }
        else
        {
            Node_f * result = minRoot;
            minRoot->remove();
            minRoot = nullptr;
            leftMostRoot = nullptr;
            numNodes -= 1;
            return result;
        }
    }
    Node_f * result = minRoot;
    minRoot->remove();
    
    minRoot = tmp;
    int size = ceil(log(numNodes) / log(2));
    for(int i = 0; i < size; i++)
        rootListByRank[i] = nullptr;
    while (tmp)
    {
        if (tmp->key < minRoot->key)
            minRoot = tmp;
        nextTmp = tmp->rightSibling;
        link(tmp);
        tmp = nextTmp;
    }
    numNodes -= 1;
    return result;
}

bool FibHeap::link(Node_f * root)
{
    if (rootListByRank[root->rank] == nullptr)
    {
        rootListByRank[root->rank] = root;
        return false;
    }
    else
    {
        Node_f * linkNode = rootListByRank[root->rank];
        rootListByRank[root->rank] = nullptr;
        
        if (root->key < linkNode->key || root == minRoot)
        {
            if (linkNode == leftMostRoot)
                leftMostRoot = linkNode->rightSibling;
            linkNode->remove();
            root->addChild(linkNode);
            if (rootListByRank[root->rank] != nullptr)
                link(root);
            else    
                rootListByRank[root->rank] = root;
        }
        else
        {
            if (root == leftMostRoot)
                leftMostRoot = root->rightSibling;
            root->remove();
            linkNode->addChild(root);
            if (rootListByRank[linkNode->rank] != nullptr)
                link(linkNode);
            else
                rootListByRank[linkNode->rank] = linkNode;
        }
        return true;
    }
}

void FibHeap::decreaseKey(Node_f * node, int newkey)
{
    node->key = newkey;
    Node_f * tmp = node;
    Node_f * parent = nullptr;
    if (node == minRoot)
        return ;
    while (tmp->parent && tmp->parent->state == LABELED)
    {
        parent = tmp->parent;
        tmp->remove();
        minRoot->addSibling(tmp);
        if (minRoot->key > tmp->key)
            minRoot = tmp;
        tmp = parent;
    }

    if (tmp->parent)
    {
        if (tmp->parent->state == UNLABELED)
        {
            tmp->parent->state = LABELED;
            tmp->remove();
            minRoot->addSibling(tmp);
            if (minRoot->key > tmp->key)
                minRoot = tmp;
        }
    }
    else 
    {
        if (minRoot->key > tmp->key)
            minRoot = tmp;
    }

}

#endif 