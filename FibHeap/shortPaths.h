#ifndef SHORTPATH_H_
#define SHORTPATH_H_

#include <iostream>
#include "fibheap.h"

#define LARGE1 9999999

using namespace std;

typedef struct node{
   struct arc *first; /* first arc in linked list */
   int id;  // The number of the vertex in this node 
   int key;  /* Distance estimate, named key to reuse heap code*/
   int P;  /* Predecessor node in shortest path */
   int position;  /* Position of node in heap, from 0 to Nm, where 0 is best, not used in FibHeap */
   } nodeitem;


void DijkstraFibHeap(nodeitem N[], int Or, int Nm)
{
   FibHeap *thisHeap; // Fibnacci Heap
   struct arc *edge;
   Node *node, *v;
   thisHeap = new FibHeap;
   Node * myNodes[MAX_SIZE];
   N[Or].key = 0;
   for (int i = 1; i <= Nm; i++)
   {
       myNodes[i] = new Node(N[i].id, N[i].key, N[i].first);
       thisHeap->insert(myNodes[i]);
   }
   while (!thisHeap->isEmpty())
   {
       node = thisHeap->removeMin();
       edge = node->first;
       while (edge != NULL)
       {
           v = myNodes[edge->end];
           if (v->key >= LARGE1 || v->key > node->key + edge->length)
           {
               thisHeap->decreaseKey(v, node->key + edge->length);
               v->P = node->id;
           }
           edge = edge->next;
       }
   }
   for (int i = 1; i <= Nm; i++)
   {
       N[i].key = myNodes[i]->key;
       N[i].P = myNodes[i]->P;
   }
        
} /* end DijkstraHeap */ 
#endif
