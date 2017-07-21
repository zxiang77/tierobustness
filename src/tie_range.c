// gcc tie_range.c -I/usr/local/include/igraph -L/usr/local/lib -ligraph -o ig_tie_range
// #include <Python.h>
// #include <numpy/arrayobject.h>
#include <igraph.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "global.h"

/* a link in the queue, holds the info and point to the next Node*/
struct data{
  igraph_integer_t info;
};

struct Node_t {
  DATA data;
  struct Node_t *prev;
};

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
struct Queue {
  NODE *head;
  NODE *tail;
  int size;
  int limit;
};

struct TreeNode {
  TreeNode **children;
  igraph_integer_t node_id;
  int f; // distance to d
}

struct ShortestTree {
    TreeNode *head;
    int size;
    int limit;
    igraph_vector_t has_path;
};

// static PyObject *ignp_fun_findNeighbors(PyObject *self, PyObject *args);

static int tie_range_BBFS(const igraph_t *g, igraph_integer_t eid,
                     int as_directed, int range_lim);
                     void build_queue_from_igraph(Queue* queue, igraph_vector_t *neighbors, int *found, igraph_vector_t *build_visited, igraph_vector_t *check_visited);

static int tie_range_BBFS(const igraph_t *g, igraph_integer_t eid,
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
  // range_lim
  const int MAX_DIST = range_lim;

  long int vcount = (long int)igraph_vcount(g);
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
  }

  return step;

}

void build_queue_from_igraph(Queue* queue, igraph_vector_t *neighbors, int *found, igraph_vector_t *build_visited, igraph_vector_t *check_visited) {
  int i;
  NODE *tmp;
  for (i=0; i<igraph_vector_size(neighbors); i++) {
    if (!VECTOR(*build_visited)[ (int)(VECTOR(*neighbors)[i]) ]) {
      // VECTOR(dimvector)[0]=30;
      if(VECTOR(*check_visited)[ (int)(VECTOR(*neighbors)[i]) ]) {*found = 1;}
      VECTOR(*build_visited)[ (int)(VECTOR(*neighbors)[i]) ] = 1;
      tmp = (NODE*) malloc(sizeof (NODE));
      tmp->data.info = (VECTOR(*neighbors)[i]);
      Enqueue(queue, tmp);
    }
  }
}

int main(int argc, char *argv[]) {
  igraph_t g;
  FILE *in_file, *out_file;
  int i, result;
  in_file = fopen(argv[1], "r");
  out_file = fopen(argv[2], "w");
  char *pre = "pre\n";
  char *post = "post\n";
  // See if the files exist
  if (in_file==0) {
  	printf("Could not open in file\n");
  	return 1;
  }
  if (out_file==0) {
  	printf("Could not open out file\n");
  	return 1;
  }
  igraph_integer_t eid = 55;

  // Read in the graphml
  if ( (result=igraph_read_graph_graphml(&g, in_file, 0)) ) {
  	printf("Trouble reading format\n");
  	return 1;
  }
  fclose(in_file);
  int tie_range = tie_range_BBFS(&g, eid, 0, 5);
  if (out_file) {
    // char out = (char) tie_range;

    char *str = malloc(5 * sizeof(char));
    sprintf(str,"%d", tie_range);
    // if (fputs(str, out_file )) {return 1;}
    fputs(str, out_file );
  	// if ( (result=igraph_write_graph_graphml(&g, out_file, 0)) ) {
  	// 	return 1;
  	// }
    	fclose(out_file);
    }
    igraph_destroy(&g);

  return 1;
}
