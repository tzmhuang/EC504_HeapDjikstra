#ifndef SHORTPATH_H_
#define SHORTPATH_H_

#include <queue>
#include "myHeap.full.h"  // This is my version of HW 2.  You can include your own binary heap

#define LARGE1 9999999

using namespace std;


struct arc{
  struct arc *next;
  int length;
  int end;
  };

typedef struct node{
   struct arc *first; /* first arc in linked list */
   int id;  // The number of the vertex in this node 
   int key;  /* Distance estimate, named key to reuse heap code*/
   int P;  /* Predecessor node in shortest path */
   int position;  /* Position of node in heap, from 0 to Nm, where 0 is best */
   } nodeitem;


void DijkstraHeap(nodeitem N[], int Or, int Nm)
{
   Heap<nodeitem> *thisHeap;
   thisHeap = new Heap<nodeitem>;
   struct arc *edge;
   nodeitem *node;
   int v, dv;
   N[Or].key = 0;

   // read nodes into heap
   for (int i=1; i<=Nm; i++){
       thisHeap->insert(&N[i]);
       N[i].position = i-1;
   }

   while (thisHeap->count() > 0){
       node = thisHeap->remove_min();
       edge = node->first;
       while (edge != NULL){
           v = edge->end;
           dv = node->key + edge->length;
           if (N[v].key > dv){
               N[v].key = dv;
               N[v].P = node->id;
               thisHeap->decreaseKey(N[v].position, dv);
           }
           edge = edge->next;
       }
   }
   // You write a Dijkstra algorithm using a binary heap; you can reuse the one from HW 2 with minor variations

} /* end DijkstraHeap */ 
#endif
