#include <stdlib.h>
#include <stdio.h>
#include <igraph.h>
#include "global.h"
#define TRUE  1
#define FALSE	0
#define UNIT_LENGTH	4
#define MAX_DEPTH	5
// typedef struct TreeNode_t TNode;
// typedef struct ShortestTree STree;

struct TreeNode_t {
  TNode **children;
  igraph_integer_t node_id;
  int f; // distance to d
  int size;
  int ptr;
};

struct ShortestTree {
  TNode *root;
  int size;
  int max_nodes;
  TNode **all_nodes;
  igraph_vector_t has_path;
  const igraph_t *g;
};
// igraph_vector_size
STree *ConstructTree(int num_nodes);
TNode *ConstructTNode(igraph_integer_t id);
void DestructTree(STree *tree);
void DestructTNode(TNode *node);
void AddChild(STree *tree, igraph_integer_t start, igraph_integer_t dest);
void AddChild(TNode *parent, TNode *child);
void build_shortest_tree(STree *tree);
void build_shortest_tree(STree *tree, int level, igraph_vector_t *visited, TNode *node);

STree *ConstructTree(igraph_t *g, int num_nodes, igraph_integer_t root){
  STree *tree = (STree*) malloc(sizeof (STree));
  if (tree == NULL) {
      return NULL;
  }
  if (num_nodes <= 0) {
      num_nodes = 65535;
  }
  TNode root = ConstructTNode(root);
  TNode **all = (TNode **) malloc(num_nodes * sizeof(TNode *));
  igraph_vector_t has_path;
  igraph_vector_init(&has_path, num_nodes);

  // TNode *root;
  // int size;
  // int max_nodes;
  // TNode **all_nodes;
  // igraph_vector_t has_path;
  tree->g = g;
  tree->root = root;
  tree->size = 1;
  tree->max_nodes = num_nodes;
  tree->all_nodes = all;
  tree->has_path = has_path;
  return tree;
}

TNode *ConstructTNode(igraph_integer_t id) {
  TNode *node = (TNode*) malloc(sizeof (TNode));

  TNode **children = (TNode **)malloc(UNIT_LENGTH * sizeof(TNode *));
  node->children = children;
  node->node_id = id;
  node->f = 0;
  node->ptr = 0;
  node->size = UNIT_LENGTH;

  return node;
}

void DestructTree(STree *tree) {
  int i;
  for(i = 0; i < tree->max_nodes; i++) {
    if(VECTOR(tree->has_path)[ (int long) i ]) {
      DestructTNode( *(node->children + i) );
    }
  }
  igraph_vector_destroy(&tree->has_path);
  free(tree);
}

void DestructTNode(TNode *node) {
  free(node);
}

void AddChild(STree *tree, igraph_integer_t start, igraph_integer_t dest) {
  TNode *s_node;
  TNode *d_node;
  if(! *(tree->all_nodes + cur_node_id)) {
    *(tree->all_nodes + cur_node_id) = ConstructTNode((igraph_integer_t)cur_node_id);
  }
  s_node = *(tree->all_nodes + (int)start);
  d_node = *(tree->all_nodes + (int) dest);
  AddChild(d_node, s_node);
  (tree->size)++;
  VECTOR(tree->has_path)[(int) start ] = 1;
}

void AddChild(TNode *parent, TNode *child) {
  if (parent->ptr == parent->size) {
    parent->children = (TNode **)realloc((parent->size + UNIT_LENGTH) * sizeof(TNode *));
    parent->size = parent->size + UNIT_LENGTH;
  }
  ptr++;
  *(node->children + node->ptr) = child;
}

void build_shortest_tree(STree *tree) {
  //  traverse through the graph to build the shortestTree
  int max_depth = 5;
  igraph_vector_t visited;
  igraph_vector_init(&visited, tree->max_nodes);
  int level = 0;

  build_shortest_tree(tree, level, &visited, tree->root);
  //
  igraph_vector_destroy(&visited);

}

void build_shortest_tree(STree *tree, int level, igraph_vector_t *visited, TNode *node) {
  if (level == MAX_DEPTH) {return;}
  igraph_vector_t neighbors;
  igraph_t *g = tree->g;
  int vcount = tree->max_nodes;
  igraph_vector_init(&neighbors, vcount);
  igraph_neighbors(g, &neighbors, node->node_id, IGRAPH_IN);
  int nneighbors = igraph_vector_size(&neighbors);
  int i ;
  for (i = 0; i < nneighbors; i++ ) {
    int long cur_node_id = VECTOR(neighbors)[ i ];
    if (!VECTOR(*visited)[(int) cur_node_id ]) {
      AddChild(tree, cur_node_id, node->node_id);
      VECTOR( *visited )[ (int) cur_node_id ] = 1;
      build_shortest_tree(tree, level + 1, &visited, *(tree->all_nodes + cur_node_id));
    }
  }
  igraph_vector_destroy(&neighbors);
}
