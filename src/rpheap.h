#ifndef RPHEAP_H_
#define RPHEAP_H_

#include <cassert>
#include <algorithm>

using namespace std;


template <typename Object>
class Node_r {
 public:
  Node_r* parent;
  Node_r* rightSibling;
  Node_r* leftChild;
  Node_r* rightChild;
  Object item;
  int rank;

  Node_r() {
    this->parent = nullptr;
    this->rightSibling = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    // this->item = nullptr;
    this->rank = 0;
  };

  Node_r(Object input) {
    this->parent = nullptr;
    this->rightSibling = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->item = input;
    this->rank = 0;
  }

  Node_r* cutRightChild() {
    if (rightChild == nullptr) return nullptr;
    rightChild->parent = nullptr;
    Node_r* tmp = rightChild;
    rightChild = nullptr;
    return tmp;
  }
};

template <typename Object>
class RPHeap {
 public:
  RPHeap() {
    minRoot = nullptr;
    heapSize = 0;
  }

  ~RPHeap();
  void insert(Node_r<Object>* node) {
    if ((node->rank) < 0) node->rank = 0;

    if (minRoot == nullptr) {
      minRoot = node;
      minRoot->rightSibling = minRoot;
    } else {
      node->rightSibling = minRoot->rightSibling;
      minRoot->rightSibling = node;
      if (node->item.key < minRoot->item.key) {
        minRoot = node;
      }
    }
    heapSize += 1;
  }

  void decreaseKey(Node_r<Object>* node, int newkey) {
    node->item.key = newkey;  // udpate key

    if (node == minRoot) {  // is minRoot
      return;
    }

    if (node->parent == nullptr) {  // one of the roots
      if (newkey < minRoot->item.key) {
        minRoot = node;
      }
      return;
    }

    // one of the children
    Node_r<Object>* nodeParent = node->parent;
    node->parent = nullptr;

    if (node == nodeParent->leftChild) {
      nodeParent->leftChild = node->rightChild;
      if (nodeParent->leftChild) {
        nodeParent->leftChild->parent = nodeParent;
      }
    } else {
      nodeParent->rightChild = node->rightChild;
      if (nodeParent->rightChild) {
        nodeParent->rightChild->parent = nodeParent;
      }
    }
    node->rightChild = node->parent = nullptr;
    node->rank = (node->leftChild) ? node->leftChild->rank + 1 : 0;
    this->insert(node);

    if (nodeParent->parent == nullptr) {  // parent ptr is a root
      node->rank = (node->leftChild) ? node->leftChild->rank + 1 : 0;
    } else {
      while (nodeParent->parent != nullptr) {  // not root
        int lr = nodeParent->leftChild ? nodeParent->leftChild->rank : -1;
        int rr = nodeParent->rightChild ? nodeParent->rightChild->rank : -1;
        int k = (abs(rr - lr) > 1) ? max(rr, lr) : max(rr, lr) + 1;
        nodeParent->rank = k;
        nodeParent = nodeParent->parent;
      }
    }
  }

  Node_r<Object>* removeMin() {
    // if (minRoot == nullptr) {
    //   cout << "empty heap error, cannot delete" << endl;
    //   return minRoot;
    // }
    int N = MaxBucketSize();
    Node_r<Object>* bucket[N];  // Initialize to nullptr
    for (int i = 0; i < N; i++)
        bucket[i] = nullptr;
    // recurse through min root right child
    {
      Node_r<Object>* p = minRoot->leftChild;
      while (p) {
        Node_r<Object>* nextp = p->rightChild;
        p->rightChild = nullptr;
        p->parent = nullptr;
        this->meld(bucket, p);
        p = nextp;
      }
    }

    {
      Node_r<Object>* p = minRoot->rightSibling;
      while (p != minRoot) {
        Node_r<Object>* nextp = p->rightSibling;
        p->rightSibling = nullptr;
        this->meld(bucket, p);
        p = nextp;
      }
    }
    Node_r<Object>* tmp = minRoot;
    assert(this->removeRoot(minRoot));
    minRoot = nullptr;

    for (int i = 0; i < N; i++) {
      Node_r<Object>* p = bucket[i];
      if (p) {
        this->insert(p);
      }
    }
    return tmp;
  }

  bool isEmpty() { return (minRoot == nullptr); }
  int getSize() { return this->heapSize; }
  Node_r<Object>* getMin() { return this->minRoot; }

 private:
  Node_r<Object>* merge(Node_r<Object>* node1, Node_r<Object>* node2) {
    // assert(node1->rightChild == nullptr && node2->rightChild == nullptr &&
    //        "Merging non half trees");
    // assert(node1->parent == nullptr && node2->parent == nullptr &&
    //        "Merging non root nodes");

    Node_r<Object>* node_up;
    Node_r<Object>* node_down;
    if ((node1->item.key) <= (node2->item.key)) {
      node_up = node1;
      node_down = node2;
    } else {
      node_up = node2;
      node_down = node1;
    }

    node_down->parent = node_up;

    if (node_up->leftChild != nullptr) {
      node_down->rightChild = node_up->leftChild;
      node_down->rightChild->parent = node_down;
    }
    node_up->leftChild = node_down;
    node_up->rank = node_down->rank + 1;

    return node_up;
  }

  bool removeRoot(Node_r<Object>* root) {
    // if (root->parent != nullptr) {
    //   cout << "Removing non-root node" << endl;
    //   return false;
    // }

    // if ((root->rightSibling) == nullptr || (root->leftSibling) == nullptr) {
    //   cout << "Removing non-attached root";
    //   return false;
    // }

    root->rightSibling = nullptr;

    if (root->leftChild != nullptr) {
      root->leftChild = nullptr;
    }
    root->rank = -1;
    this->heapSize -= 1;
    return true;
  }

  void meld(Node_r<Object>** bucket, Node_r<Object>* p) {
    while (bucket[p->rank] != nullptr) {
      int rank = p->rank;
      p = this->merge(bucket[rank], p);

      bucket[rank] = nullptr;
    }

    bucket[p->rank] = p;
  }

  inline int MaxBucketSize()  // ceil(log2(size)) + 1
  {
    int bit = 1, count = heapSize;
    while (count >>= 1) bit++;
    return bit + 1;
  }

  Node_r<Object>* minRoot;
  int heapSize = 0;
};
#endif
