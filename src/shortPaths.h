#ifndef SHORTPATH_H_
#define SHORTPATH_H_

#include <queue>
//#include "myHeap.full.h"  // This is my version of HW 2.  You can include your own binary heap
#include "violationHeap.hpp"
#include "arc.h"
#include "rpheap.h"
#include "fibheap.h"

#define LARGE1 9999999

using namespace std;

typedef struct node_d {
    struct arc *first; /* first arc in linked list */
    int id;            // The number of the vertex in this node
    int key;           /* Distance estimate, named key to reuse heap code*/
    int P;             /* Predecessor node in shortest path */
    int position; /* Position of node in heap, from 0 to Nm, where 0 is best */
} nodeitem;



void DijkstraFibHeap(Node_f *myNodes[], int Or, int Nm) {
    FibHeap *thisHeap; // Fibnacci Heap
    struct arc *edge;
    Node_f *node, *v;
    thisHeap = new FibHeap;
    myNodes[Or]->key = 0;
    for (int i = 1; i <= Nm; i++) {
        thisHeap->insert(myNodes[i]);
    }
    while (!thisHeap->isEmpty()) {
        node = thisHeap->removeMin();
        edge = node->first;
        while (edge != NULL) {
            v = myNodes[edge->end];
            if (v->key >= LARGE1 || v->key > node->key + edge->length) {
                thisHeap->decreaseKey(v, node->key + edge->length);
                v->P = node->id;
            }
            edge = edge->next;
        }
    }
}

void DijkstraRPHeap(nodeitem N[], int Or, const int Nm) {
    RPHeap<nodeitem> *thisHeap;
    thisHeap = new RPHeap<nodeitem>;
    struct arc *edge;
    Node_r<nodeitem> *node;
    Node_r<nodeitem> test;
    nodeitem test1;
    int v, dv;
    N[Or].key = 0;

    auto *HN = new Node_r<nodeitem>[Nm + 1];
    for (int i = 0; i <= Nm; i++) {
        HN[i] = Node_r<nodeitem>(N[i]);
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
}

void DijkstraHeap(node_t *N[], int Or, int Nm)
{
   auto *thisHeap = make_heap();
   struct arc *edge;
   node_t *node;
   
   // You write a Dijkstra algorithm using a binary heap; you can reuse the one from HW 2 with minor variations

   auto **current_key = static_cast<node_t **>(calloc(Nm + 1, sizeof(node_t *)));

   N[Or]->key = 0;
   for (int i = 1; i <= Nm; i++)
       current_key[i] = insert(thisHeap ,N[i]);

   while(thisHeap->size != 0){
       node = extract_min(thisHeap);
       edge = node->first;
       while (edge != nullptr){
           if(N[edge->end]->key > node->key + edge->length){
               decrease_key(thisHeap, N[edge->end], node->key + edge->length);
               N[edge->end]->P = node->id;
           }
           edge = edge->next;
       }
   }

   delete thisHeap;

} /* end DijkstraHeap */ 
#endif
