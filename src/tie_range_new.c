#include <igraph.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"

static int tie_range_BBFS(const igraph_t *g, igraph_integer_t eid,\
                     int as_directed, int range_lim);

static igraph_heap_t *k_shortest(const igraph_t *g);

static int tie_range_BBFS(const igraph_t *g, igraph_integer_t eid,\
                    int as_directed, int range_lim) {
  igraph_integer_t s_node; // id of source
  igraph_integer_t d_node; // id of dest

  igraph_edge( g, eid, &s_node, &d_node );

  igraph_vector_t s_visited;
  igraph_vector_t d_visited;

  igraph_dqueue_t s_node_queue;
  igraph_dqueue_t d_node_queue; // nodes on current layer

  igraph_vector_t s_neighbors;
  igraph_vector_t d_neighbors;

  igraph_vector_t neighbors;
  igraph_vector_t visited;
  igraph_vector_t intersection;
  int intersection_is_set = 0;

  bool step = 0;
  // range_lim
  const int MAX_DIST = range_lim;

  long int vcount = (long int)igraph_vcount(g);

  igraph_dqueue_init(&s_node_queue, vcount);
  igraph_dqueue_init(&d_node_queue, vcount);

  // init start and end nodes
  igraph_vector_init(&s_visited, vcount);
  igraph_vector_init(&d_visited, vcount);

  igraph_vector_init(&s_neighbors, vcount);
  igraph_vector_init(&d_neighbors, vcount);

  igraph_vector_init(&intersection, vcount);

  igraph_vector_fill(&s_visited, 0);
  igraph_vector_fill(&d_visited, 0);

  igraph_vector_init(&visited, vcount);
  igraph_vector_fill(&visited, -1);

  VECTOR(s_visited)[ (int long) s_node ] = 1;
  VECTOR(d_visited)[ (int long) d_node ] = 1;

  VECTOR(visited)[ (int long) s_node ] = 1;
  VECTOR(visited)[ (int long) d_node ] = 1;

  igraph_vector_init(&visited, vcount);
  // if node on current layer is null quit
  int s_num_neighbors = 0;
  int d_num_neighbors = 0;
  struct Queue *s_queue = ConstructQueue(vcount);
  struct Queue *d_queue = ConstructQueue(vcount);

  NODE *s_cur;
  s_cur = (NODE*) malloc(sizeof (NODE));
  s_cur->data.info = s_node;
  // igraph_integer_t s_node;
  Enqueue(s_queue, s_cur);

  NODE *d_cur;
  d_cur = (NODE*) malloc(sizeof (NODE));
  d_cur->data.info = d_node;
  Enqueue(d_queue, d_cur);

  int isFrom = 1;
  struct Queue *s_tmp;
  struct Queue *d_tmp;
  NODE *tmp_node;
  int found = 0;

  while(step < MAX_DIST && !intersection_is_set) {
   step++;
   if(isFrom == 1) { isFrom = 0; }
   if(isFrom == 0) { isFrom = 1; }

   if(isFrom) {
     s_tmp = s_queue;
     s_queue = ConstructQueue(vcount);

     while(!isEmpty(s_tmp)) {
       tmp_node = Dequeue(s_tmp);
       igraph_vector_init(&neighbors, vcount);
       igraph_neighbors(g, &neighbors, tmp_node->data.info, IGRAPH_IN);
       build_queue_from_igraph(s_queue, &neighbors, &found, &s_visited, &d_visited);
       igraph_vector_destroy(&neighbors);
     }

     DestructQueue(s_tmp);
   } else {
     d_tmp = d_queue;
     d_queue = ConstructQueue(vcount);

     while(!isEmpty(d_tmp)) {
       tmp_node = Dequeue(d_tmp);
       igraph_vector_init(&neighbors, vcount);
       igraph_neighbors(g, &neighbors, tmp_node->data.info, IGRAPH_OUT);
       build_queue_from_igraph(d_queue, &neighbors, &found, &d_visited, &s_visited);
       igraph_vector_destroy(&neighbors);
     }

     DestructQueue(d_tmp);
   }
   if(found) break;

  igraph_dqueue_destroy(&s_node_queue);
  igraph_dqueue_destroy(&d_node_queue);

  return step;

}

static igraph_heap_t *k_shortest(const igraph_t *g, igraph_integer_t dest){
    // TODO:
    // igraph heap only add a real number but not an object
    // the real number is not unique, cannot have one to one mapping
    // keep a many to many mapping object
    // or still need a heap class for the successful functionality of k shortest
    //
    igraph_heap_t dest_heap;

    // tree
    igraph_tree_t

    long int vcount = (long int)igraph_vcount(g);
    igraph_heap_init(&dest_heap, vcount);
}
