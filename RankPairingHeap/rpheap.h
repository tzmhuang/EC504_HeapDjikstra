#ifndef RPHEAP_H_
#define RPHEAP_H_

#include <assert.h>

using namespace std;

const int MAX_SIZE =
    200000;  // the maximum amount of elements our heap should have.

template <typename Object>
class Node {
 public:
  Node* parent;
  Node* leftSibling;
  Node* rightSibling;
  Node* leftChild;
  Node* rightChild;
  Object item;
  int rank;

  Node() {
    this->parent = nullptr;
    this->leftSibling = nullptr;
    this->rightSibling = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    // this->item = nullptr;
    this->rank = 0;
  };

  Node(Object input) {
    this->parent = nullptr;
    this->leftSibling = nullptr;
    this->rightSibling = nullptr;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->item = input;
    this->rank = 0;
  }

  Node* cutRightChild() {
    if (rightChild == nullptr) return nullptr;
    rightChild->parent = nullptr;
    Node* tmp = rightChild;
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
  void insert(Node<Object>* node) {
    if ((node->rank) < 0) node->rank = 0;

    if (minRoot == nullptr) {
      minRoot = node;
      minRoot->leftSibling = minRoot;
      minRoot->rightSibling = minRoot;
    } else {
      minRoot->rightSibling->leftSibling = node;
      node->rightSibling = minRoot->rightSibling;
      minRoot->rightSibling = node;
      node->leftSibling = minRoot;
      if (node->item.key < minRoot->item.key) {
        minRoot = node;
      }
    }
    numRoot += 1;
    heapSize += 1;
  }

  void decreaseKey(Node<Object>* node, int newkey) {
    assert(newkey < (node->item.key) && "Invalid decreaseKey");
    node->item.key = newkey;  // udpate key
    
    if (node == minRoot){ // is minRoot
      return;
    }

    if (node->parent == nullptr) { // one of the roots
      if (newkey < minRoot->item.key) {
        minRoot = node;
      }
      return;
    }

    //one of the children
    Node<Object>* nodeParent = node->parent;
    node->parent = nullptr;

    if (node == nodeParent->leftChild){
      nodeParent->leftChild = node->rightChild;
      if (nodeParent->leftChild){
        nodeParent->leftChild->parent = nodeParent;
      }
    }else{
      nodeParent->rightChild = node->rightChild;
      if(nodeParent->rightChild){
        nodeParent->rightChild->parent = nodeParent;
      }
    }

    node->rightChild = node->parent = nullptr;
    node->rank = (node->leftChild) ? node->leftChild->rank + 1 : 0;
    this->insert(node);

    if (nodeParent->parent == nullptr){ //parent ptr is a root
      node->rank = (node->leftChild) ? node->leftChild->rank + 1 : 0;
    }else{
      while (nodeParent->parent != nullptr) { // not root
        int lr = nodeParent->leftChild ? nodeParent->leftChild->rank : -1;
        int rr = nodeParent->rightChild ? nodeParent->rightChild->rank : -1;
        int k = (abs(rr - lr) > 1) ? max(rr, lr) : max(rr, lr) + 1;
        nodeParent->rank = k;
        nodeParent = nodeParent->parent;
      }
    }

  }

  Node<Object>* removeMin() {
    if (minRoot == nullptr) {
      cout << "empty heap error, cannot delete" << endl;
      return minRoot;
    }
    Node<Object>* tmprc = nullptr;
    Node<Object>* tmp = minRoot;
    Node<Object>* p = minRoot->leftChild;
    if (minRoot->rightSibling == minRoot) {
      minRoot = nullptr;  // sibling points to self
    } else {
      minRoot = minRoot->rightSibling;
    }
    assert(this->removeRoot(tmp));

    // recursive cut right and update rank
    while (p != nullptr) {
      tmprc = p->cutRightChild();
      p->rank = (p->leftChild) ? p->leftChild->rank + 1 : 0;
      this->insert(p);
      p = tmprc;
    }
    this->collapse();

    // this->updateMin();
    return tmp;
  }

  bool isEmpty() { return (minRoot == nullptr); }
  int getSize() { return this->numRoot; }
  Node<Object>* getMin() { return this->minRoot; }

 private:
  Node<Object>* merge(Node<Object>* node1, Node<Object>* node2) {
    // assert(node1->rightChild == nullptr && node2->rightChild == nullptr &&
    //        "Merging non half trees");
    // assert(node1->parent == nullptr && node2->parent == nullptr &&
    //        "Merging non root nodes");
    Node<Object>* tmp = nullptr;

    if ((node1->item.key) <= (node2->item.key)) {
      tmp = node1;
      node1 = node2;
      node2 = tmp;
    }

    if (node2->leftChild != nullptr) {
      node1->rightChild = node2->leftChild;
      node2->leftChild->parent = node1;
    }
    node2->leftChild = node1;
    node1->parent = node2;
    if (node1 == minRoot) {
      minRoot = node2;
    }
    node1->leftSibling->rightSibling = node1->rightSibling;
    node1->rightSibling->leftSibling = node1->leftSibling;
    node1->leftSibling = nullptr;
    node1->rightSibling = nullptr;
    this->numRoot -= 1;

    node2->rank += 1;
    return node2;
  }

  void updateMin() {
    Node<Object>* p = minRoot;

    for (int i = 0; i < numRoot; i++) {
      p = p->rightSibling;
      if (p->item.key < minRoot->item.key) minRoot = p;
    }
  }

  bool removeRoot(Node<Object>* root) {
    if (root->parent != nullptr) {
      cout << "Removing non-root node" << endl;
      return false;
    }

    if ((root->rightSibling) == nullptr || (root->leftSibling) == nullptr) {
      cout << "Removing non-attached root";
      return false;
    }
    this->numRoot -= 1;

    root->rightSibling->leftSibling = root->leftSibling;
    root->leftSibling->rightSibling = root->rightSibling;
    root->leftSibling = nullptr;
    root->rightSibling = nullptr;

    if (root->leftChild != nullptr) {
      root->leftChild->parent = nullptr;
      root->leftChild = nullptr;
    }
    root->rank = -1;
    this->heapSize -= 1;
    return true;
  }

  void collapse() {
    Node<Object>* ptr = minRoot;
    Node<Object>* rankArray[MAX_SIZE] = {nullptr};  // Initialize to nullpitr
    int N = numRoot;

    for (int i = 0; i < N; i++) {
      Node<Object>* p = ptr;
      ptr = ptr->rightSibling;
      // Multi_pass
      while (rankArray[p->rank] != nullptr) {
        int rank = p->rank;
        p = this->merge(rankArray[rank], p);
        rankArray[rank] = nullptr;
      }

      rankArray[p->rank] = p;
      // Multipass end
      if (p->item.key < minRoot->item.key) minRoot = p;
    }
  }

  inline int MaxBucketSize()  // ceil(log2(size)) + 1
  {
    int _Bit = 1, _Count = heapSize;
    while (_Count >>= 1) _Bit++;
    return _Bit + 1;
  }

  Node<Object>* minRoot;
  int numRoot = 0;
  int heapSize = 0;
};
#endif
