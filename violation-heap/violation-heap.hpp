#ifndef VIOLATION_HEAP_HEADER
#define VIOLATION_HEAP_HEADER

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>

using namespace std;

#define SWAP_NODE(a, b) 		\
do {					\
	node_t *temp = a;		\
	(a) = b;				\
	(b) = temp;			\
}while(0)

/* Define the type of key to be int */
typedef int key_t;

/* Define arc */
struct arc{
    struct arc *next;
    int length;
    int end;
};

/* Define the node struct */
typedef struct node {
	key_t key;
	int rank;
    int id;
    int P;
    int position;
    struct arc *first;
	struct node *next;
	struct node *prev;
	struct node *child;
} node_t;

/* Define the heap struct */
typedef struct heap {
	int size;
	struct node *root_list;
} heap_t;

heap_t *make_heap();
node_t *make_node(key_t key);
node_t *insert(heap_t *heap, node_t *node);
node_t *extract_min(heap_t *heap);
void decrease_key(heap_t *heap, node_t *node, key_t new_key);
void free_nodes(node_t *N[], int Nm);


#endif
