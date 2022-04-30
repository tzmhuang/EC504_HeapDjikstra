#ifndef SHORTPATH_H_
#define SHORTPATH_H_

#include <queue>
#include "rpheap.h"

#define maxnodes 200000
#define LARGE1 9999999

using namespace std;

struct arc {
  struct arc *next;
  int length;
  int end;
};

typedef struct node {
  struct arc *first; /* first arc in linked list */
  int id;            // The number of the vertex in this node
  int key;           /* Distance estimate, named key to reuse heap code*/
  int P;             /* Predecessor node in shortest path */
  int position; /* Position of node in heap, from 0 to Nm, where 0 is best */
} nodeitem;

void DijkstraRPHeap(nodeitem N[], int Or, const int Nm) {
  RPHeap<nodeitem> *thisHeap;
  thisHeap = new RPHeap<nodeitem>;
  struct arc *edge;
  Node<nodeitem> *node;
  Node<nodeitem> test;
  nodeitem test1;
  int v, dv;
  N[Or].key = 0;

  Node<nodeitem> *HN = new Node<nodeitem> [Nm+1];
  for (int i = 0; i <= Nm; i++) {
    HN[i] = Node<nodeitem> (N[i]);  
  }

  // read nodes into heap
  for (int i = 1; i <= Nm; i++) {
    thisHeap->insert(&HN[i]);
  } 

  while (!thisHeap->isEmpty()) {

    node = thisHeap->removeMin();
    edge = node->item.first;
    while (edge != NULL) {
      v = edge->end;
      dv = node->item.key + edge->length;
      if (N[v].key > dv) {
        N[v].key = dv;
        N[v].P = node->item.id;
        thisHeap->decreaseKey(&HN[v], dv);
      }
      edge = edge->next;
    }
  }

} /* end DijkstraRPHeap */
#endif
