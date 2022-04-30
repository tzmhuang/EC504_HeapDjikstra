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

  int updateRank() {
    int diff, maxr, lr, rr;
    if (parent == nullptr) {
      this->rank = (leftChild == nullptr) ? 0 : leftChild->updateRank() + 1;
      return this->rank;
    }

    if (rightChild == nullptr && leftChild == nullptr) {
      this->rank = 0;
    } else {
      lr = (leftChild == nullptr) ? 0 : leftChild->updateRank();
      rr = (rightChild == nullptr) ? 0 : rightChild->updateRank();
      diff = abs(lr - rr);
      maxr = max(lr, rr);
      if (diff > 1)
        this->rank = maxr;
      else
        this->rank = maxr + 1;
    }
    return this->rank;
  }
};

template <typename Object>
class RPHeap {
 public:
  RPHeap() { minRoot = nullptr; }

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
  }

  void decreaseKey(Node<Object>* node, int newkey) {
    assert(newkey < (node->item.key) && "Invalid decreaseKey");
    Node<Object>* nodeParent = node->parent;
    node->item.key = newkey;  // udpate key
    if (nodeParent == nullptr) {
      if (newkey < minRoot->item.key) {
        minRoot = node;
      }
      return;
    }
    node->parent = nullptr;
    if (nodeParent->rightChild == node) {
      nodeParent->rightChild = node->rightChild;
    } else {
      nodeParent->leftChild = node->rightChild;
    }

    if (node->rightChild != nullptr) {
      node->rightChild->parent = nodeParent;
      node->rightChild = nullptr;
    }

    // node->updateRank();
    node->rank = (node->leftChild) ? node->leftChild->rank + 1 : 0;
    this->insert(node);

    while (nodeParent->parent != nullptr) {
      nodeParent = nodeParent->parent;
    }

    nodeParent->updateRank();
    if (newkey < minRoot->item.key) {
      minRoot = node;
    }
  };

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

    this->updateMin();
    return tmp;
  }

  bool isEmpty() { return (minRoot == nullptr); }
  int getSize() { return this->numRoot; }
  Node<Object>* getMin() {return this->minRoot;}

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
    if(node1 == minRoot){
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
    return true;
  }

  void collapse() {
    Node<Object>* ptr = minRoot;
    Node<Object>* rankArray[MAX_SIZE] = {nullptr};  // Initialize to nullpitr
    int N = numRoot;

    for (int i = 0; i < N; i++) {
      Node<Object>* p = ptr;
      ptr = ptr->rightSibling;
      if (rankArray[p->rank] == nullptr) {
        rankArray[p->rank] = p;
        continue;
      }
      while (rankArray[p->rank] != nullptr) {
        int rank = p->rank;
        p = this->merge(rankArray[rank], p);
        rankArray[rank] = nullptr;
      }

      rankArray[p->rank] = p;
      if (p->item.key < minRoot->item.key) minRoot = p;

    }
  }
  Node<Object>* minRoot;
  int numRoot = 0;
};
#endif
