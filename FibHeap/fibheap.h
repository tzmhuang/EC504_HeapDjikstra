#ifndef FIBHEAP_H_
#define FIBHEAP_H_

#include <vector>

using namespace std;
const int MAX_SIZE = 200000;


enum State {
    LABELED, UNLABELED, SCANNED
};


struct arc{
  struct arc *next;
  int length;
  int end;
};

class Node {
public:
    Node * parent;
    Node * leftSibling, * rightSibling;
    Node * children;

    Node(int id, int key, struct arc * first);
    Node();

    struct arc *first; /* first arc in linked list */
    int id; /* Node id */
    int key;  /* Distance estimate, named key to reuse heap code*/
    int rank;
    int P; /* Predecessor node in shortest path */

    State state;

    bool addChild(Node * node);
    bool addSibling(Node * node);
    bool remove();

    Node * leftMostSibling();
    Node * rightMostSibling();

};


class FibHeap {
private:
    Node * rootListByRank[MAX_SIZE];
    Node * minRoot;
    bool link(Node * root);

public:
    FibHeap();
    FibHeap(Node * root);
    ~FibHeap();
    bool isEmpty();
    void decreaseKey(Node * node, int newkey);
    bool insert(Node * node);
    Node * findMin();
    Node * removeMin();
};

Node::Node()
{
    this->parent = nullptr;
    this->children = nullptr;
    this->leftSibling = nullptr;
    this->rightSibling = nullptr;
    this->rank = 0;
    this->state = UNLABELED;
}

Node::Node(int id, int key, struct arc * first)
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

bool Node::addChild(Node * node)
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

bool Node::addSibling(Node * node)
{
    Node * tmp = rightMostSibling();
    if (tmp == nullptr)
        return false;
    tmp->rightSibling = node;
    node->leftSibling = tmp;
    node->parent = this->parent;
    node->rightSibling = nullptr;

    if (this->parent)
        this->parent->rank++;
    return true;
}

bool Node::remove()
{
    if (parent)
    {
        parent->rank--;
        if (leftSibling)
            parent->children = leftSibling;
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

Node* Node::leftMostSibling()
{
    if (this == nullptr)
        return nullptr;
    Node * tmp = this;
    while (tmp->leftSibling)
        tmp = tmp->leftSibling;
    return tmp;
}

Node * Node::rightMostSibling()
{
    if (this == nullptr)
        return nullptr;
    Node * tmp = this;
    while (tmp->rightSibling)
        tmp = tmp->rightSibling;
    return tmp;
}

FibHeap::FibHeap()
{
    minRoot = nullptr;
}

FibHeap::~FibHeap()
{
    delete[] this->rootListByRank;
}

bool FibHeap::isEmpty()
{
    return (minRoot == nullptr);
}

bool FibHeap::insert(Node * node)
{
    if (node == nullptr)
        return false;
    if (minRoot == nullptr)
        minRoot = node;
    else
    {
        minRoot->addSibling(node);
        if (minRoot->key > node->key)
            minRoot = node;
    }
    return true;
}

Node * FibHeap::findMin()
{
    return minRoot;
}

Node * FibHeap::removeMin()
{
    Node * tmp = minRoot->children->leftMostSibling();
    Node * nextTmp = nullptr;

    while (tmp != nullptr)
    {
        nextTmp = tmp->rightSibling;
        tmp->remove();
        minRoot->addSibling(tmp);
        tmp = nextTmp;
    }

    tmp = minRoot->leftMostSibling();
    if (tmp == minRoot)
    {
        if (minRoot->rightSibling)
            tmp = minRoot->rightSibling;
        else
        {
            Node * result = minRoot;
            minRoot->remove();
            minRoot = nullptr;
            return result;
        }
    }
    Node * result = minRoot;
    minRoot->remove();
    
    minRoot = tmp;
    for(int i = 0; i < MAX_SIZE; i++)
        rootListByRank[i] = nullptr;
    while (tmp)
    {
        if (tmp->key < minRoot->key)
            minRoot = tmp;
        nextTmp = tmp->rightSibling;
        link(tmp);
        tmp = nextTmp;
    }
    return result;
}

bool FibHeap::link(Node * root)
{
    if (rootListByRank[root->rank] == nullptr)
    {
        rootListByRank[root->rank] = root;
        return false;
    }
    else
    {
        Node * linkNode = rootListByRank[root->rank];
        rootListByRank[root->rank] = nullptr;
        
        if (root->key < linkNode->key || root == minRoot)
        {
            linkNode->remove();
            root->addChild(linkNode);
            if (rootListByRank[root->rank] != nullptr)
                link(root);
            else    
                rootListByRank[root->rank] = root;
        }
        else
        {
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

void FibHeap::decreaseKey(Node * node, int newkey)
{
    node->key = newkey;
    Node * tmp = node;
    Node * parent = nullptr;
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