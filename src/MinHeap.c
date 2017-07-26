#include <stdlib.h>
#include <stdio.h>
#include <igraph.h>
#include "global.h"
#define INIT_SIZE 255
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2
/* data of heap are paths, order paths by its f value */
struct Path_t {
  int length;
  TNode *path;
};

struct MinHeap_t {
  Path *paths;
  int ptr;
  int size;
};

// level 1->1, 2->3, 3->
MinHeap *ConstructHeap();
void DestructHeap(MinHeap *heap);
void AddHeap(MinHeap *heap, Path *path);
Path *HeapPop(MinHeap *heap);
void SiftUp(MinHeap *heap, int node_index);
void Swap(Path *p1, Path *p2);

MinHeap *ConstructHeap() {
  MinHeap *heap = malloc(sizeof(MinHeap));
  heap->paths = malloc(INIT_SIZE * sizeof(Path));
  heap->ptr = 0;
  heap->size = INIT_SIZE;
  return heap;
}

void DestructHeap(MinHeap *heap){
  int i;
  Path *tmp;
  for (i = 0; i < heap->ptr; i++) {
    if(heap->paths + i) {
      tmp = heap->paths + i;
      free(tmp);
    }
  }
  free(heap);
}

void AddHeap(MinHeap *heap, Path *path){

  if(heap->ptr == heap->size) {
    heap->paths = realloc(heap->paths, heap->size * 2 + 1 );
  }
  Path *tmp = (heap->paths + heap->ptr);
  tmp = path;
  SiftUp(heap, heap->ptr);
  (heap->ptr)++;
}
// #define LCHILD(x) 2 * x + 1
// #define RCHILD(x) 2 * x + 2
// #define PARENT(x) (x - 1) / 2
Path *HeapPop(MinHeap *heap){
  Path *pop = heap->paths;
  heap->paths = NULL;
  (heap->ptr)--;
  Swap(heap->paths, heap->paths + heap->ptr);
  SiftUp(heap, heap->ptr);
  return pop;
}

void SiftUp(MinHeap *heap, int node_index) {
  // int node_index = heap->ptr - 1;
  int parent_index, tmp;
  if (node_index != 0) {
        parent_index = PARENT(node_index);
        if((heap->paths + parent_index)->length > (heap->paths + node_index)->length) {
              Swap( heap->paths + parent_index, heap->paths + node_index);
              SiftUp(heap, parent_index);
        }
  }
}

void Swap(Path *p1, Path *p2) {
    Path temp = *p1 ;
    *p1 = *p2 ;
    *p2 = temp ;
}
