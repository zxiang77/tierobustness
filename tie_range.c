// gcc tie_range.c -I/usr/local/include/igraph -L/usr/local/lib -ligraph -o ig_tie_range
#include <Python.h>
#include <numpy/arrayobject.h>
#include <igraph.h>
#include <stdbool.h>
#include "global.h"
// function signature
typedef struct node {
  node *next;
  int long val;
} node;

static PyObject *ignp_fun_findNeighbors(PyObject *self, PyObject *args);
static int tie_range_BBFS(const igraph_t *g, igraph_integer_t eid, igraph_vector_t *visited,
                     igraph_vector_t *node_queue, igraph_vector_t *neighbors,
                     igraph_vector_t *deg, igraph_vector_t *nodes,
                     int as_directed, int range_lim);
void build_queue_from_igraph(struct *Queue queue, igraph_vector_t *neighbors,
                             igraph_vector_t *visited);
// implement a bidirectional bfs solution
int count_steps(const igraph_t *g, igraph_integer_t eid) {
  igraph_integer_t s_node;
  igraph_integer_t d_node;

  igraph_vector_t visited;
  igraph_vector_t node_queue;
  igraph_vector_t neighbors;

  long int vcount;
  long int wave_end;
  long int wave_begin;
  long int last_pos;
  long int i;
  long int j;
  long int step;
  int found;

  igraph_edge(g, eid, &s_node, &d_node);

  vcount = (long int) igraph_vcount(g);
  igraph_vector_init(&visited, vcount);

  igraph_vector_init(&node_queue, vcount);
  igraph_vector_fill(&node_queue, -1);

  igraph_vector_init(&neighbors, 0);
  igraph_neighbors(g, &neighbors, s_node, IGRAPH_ALL);

  igraph_vector_search( &neighbors, 0, d_node, &i);
  igraph_vector_remove( &neighbors, i);

  wave_end = 0;
  wave_begin = 0;
  last_pos = 0;
  found = 0;

  VECTOR(visited)[ (int)s_node ] = 1;
  for (i=0; i < igraph_vector_size(&neighbors); i++) {
    VECTOR(visited)[ (int)(VECTOR(neighbors)[i]) ] = 1;
    VECTOR(node_queue)[i] = (VECTOR(neighbors)[i]);
    wave_end += 1;
    last_pos += 1;
  }

  step = 1;

}


static int tie_range_BBFS(const igraph_t *g, igraph_integer_t eid, igraph_vector_t *visited,
                     igraph_vector_t *node_queue, igraph_vector_t *neighbors,
                     igraph_vector_t *deg, igraph_vector_t *nodes,
                     int as_directed, int range_lim) {
  igraph_integer_t s_node; // id of source
  igraph_integer_t d_node; // id of dest

  igraph_edge( g, eid, &s_node, &d_node );

  igraph_vector_t s_visited;
  igraph_vector_t s_node_queue;
  igraph_vector_t s_neighbors;

  igraph_vector_t d_visited;
  igraph_vector_t d_node_queue; // nodes on current layer
  igraph_vector_t d_neighbors;

  igraph_vector_t neighbors;
  igraph_vector_t visited;
  igraph_vector_t intersection;
  int intersection_is_set = 0;


  bool step = 0;
  const int MAX_DIST = 6;

  // compute source and destination



  // hanging check could be ingnored due to BBFS

  vcount = (long int)igraph_vcount(g);
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

  VECTOR(*s_visited)[ (int long) s_node ] = 1;
  VECTOR(*d_visited)[ (int long) d_node ] = 1;

  VECTOR(*visited)[ (int long) s_node ] = 1;
  VECTOR(*visited)[ (int long) d_node ] = 1;

  igraph_vector_init(&visited, vcount);
  // if node on current layer is null quit
  int s_num_neighbors = 0;
  int d_num_neighbors = 0;
  struct Queue *s_queue = ConstructQueue(vcount);
  struct Queue *d_queue = ConstructQueue(vcount);

  struct NODE *s_cur;
  s_cur = (NODE*) malloc(sizeof (NODE));
  s_cur->data.info = s_node;
  // igraph_integer_t s_node;
  Enqueue(s_queue, s_cur);

  struct NODE *d_cur;
  d_cur = (NODE*) malloc(sizeof (NODE));
  d_cur->data.info = d_node;
  Enqueue(d_queue, d_cur);

  int isFrom = 1;
  struct Queue *tmp;
  NODE *tmp_node;
  while(step < MAX_DIST && !intersection_is_set) {
    step++;
    if(isFrom == 1) { isFrom = 0; }
    if(isFrom == 0) { isFrom = 1; }

    if(isFrom) {
      // s_visited;
      // still need a set with vid
      // need an array to check if it has been taken, need a queue
      // to store not previously visited Vertices
      tmp = s_queue;

      s_queue = ConstructQueue(vcount);

      while(!isEmpty(tmp)) {
        tmp_node = Dequeue(tmp);
        igraph_vector_init(&neighbors, vcount);
        igraph_neighbors(g, &neighbors, tmp_node->data.info, IGRAPH_IN);
        build_queue_from_igraph(s_queue, &neighbors, s_visited);
        igraph_vector_destroy(&neighbors);
      }

      DestructQueue(tmp);
    } else {
      tmp = d_queue;
      d_queue = ConstructQueue(vcount);

      while(!isEmpty(tmp)) {
        tmp_node = Dequeue(tmp);
        igraph_vector_init(&neighbors, vcount);
        igraph_neighbors(g, &neighbors, tmp_node->data.info, IGRAPH_OUT);
        build_queue_from_igraph(d_queue, &neighbors, d_visited);
        igraph_vector_destroy(&neighbors);
      }

      DestructQueue(tmp);
    }

  }

}

void build_queue_from_igraph(struct *Queue queue, igraph_vector_t *neighbors, igraph_vector_t *visited) {
  int i;
  NODE *tmp;
  for (i=0; i<igraph_vector_size(neighbors); i++) {
    if (!VECTOR(visited)[ (int)(VECTOR(neighbors)[i]) ]) {
      VECTOR(visited)[ (int)(VECTOR(neighbors)[i]) ] = 1;
      tmp = (NODE*) malloc(sizeof (NODE));
      tmp->data.info = (VECTOR(neighbors)[i]);
      Enqueue(queue, tmp);
    }
  }
}
