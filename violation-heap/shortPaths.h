#ifndef SHORTPATH_H_
#define SHORTPATH_H_

#include <queue>
//#include "myHeap.full.h"  // This is my version of HW 2.  You can include your own binary heap
#include "violation-heap.hpp"

#define LARGE1 9999999

using namespace std;

/*
struct arc{
  struct arc *next;
  int length;
  int end;
  };
*/

/*
typedef struct node_b {
    struct arc *first; //first arc in linked list
    int id;  // The number of the vertex in this node
    int key;  //Distance estimate, named key to reuse heap code
    int P;  // Predecessor node in shortest path
    int position;  // Position of node in heap, from 0 to Nm, where 0 is best
} nodeitem;
*/
/*
void BellmanFord(nodeitem N[], int Or, int Nm)
{
   // You program this, a  Bellman Ford algorithm that uses a work queue.  DO NOT implement this as 3 for loops. 
   // That can be very slow, and waste many iterations.

   auto *Q = new queue<nodeitem *>;
   nodeitem *node_m;
   struct arc *edge;
   N[Or].key = 0;
   Q->push(&N[Or]);
   N[Or].position = 1;
   while (!Q->empty()) {
       node_m = Q->front();
       Q->pop();
       node_m->position = -1;
       edge = node_m->first;
       while(edge != nullptr){
           if(N[edge->end].key > node_m->key + edge->length) {
               N[edge->end].key = node_m->key + edge->length;
               N[edge->end].P = node_m->id;
               if(N[edge->end].position < 0) {
                   Q->push(&N[edge->end]);
                   N[edge->end].position = 1;
               }
           }
           edge = edge->next;
       }

       delete Q;
   }

}
*/

 /* end Bellman-Ford */
/* ---------------*/



void Dijkstra(node_t *N[], int Or, int Nm)
{
    int Mark[Nm+1];
    struct arc *edge;
    int v, dv, min_d, min_v, finished;
    for (int i=1; i<=Nm; i++){
        Mark[i]=-1;
    }
    N[Or]->key = 0;
    Mark[Or] = 1;
    finished = 1;
    min_v = Or;
    min_d = 0;
    while (finished < Nm){
        edge = N[min_v]->first;  //update distances
        while (edge != NULL){ // explore the outgoing arcs of u 
            v = edge->end;
            dv = min_d + edge->length;
            if (N[v]->key > dv){
                N[v]->key = dv;
                N[v]->P = min_v;
            }//if D > dv 
            edge = edge->next;
        }// while edge           
        
        min_d = LARGE1;
        for (int j = 0; j <= Nm; j++){
            if (Mark[j] < 1){
                if (N[j]->key < min_d){
                    min_d = N[j]->key;
                    min_v = j;
                }
            }
        } 
        Mark[min_v]=1;
        finished++;
    } 
}

 /* end Dijkstra */


void DijkstraHeap(node_t *N[], int Or, int Nm)
{
   auto *thisHeap = make_heap();
   struct arc *edge;
   node_t *node;
   
   // You write a Dijkstra algorithm using a binary heap; you can reuse the one from HW 2 with minor variations

   auto **current_key = static_cast<node_t **>(calloc(Nm, sizeof(node_t *)));

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
