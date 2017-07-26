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
/**
  * build a ShortestTree, which build a tree with MAX_DEPTH max depth
  * of shortest path to destination d
  */

struct TreeNode_t {
  //false
  TNode *children;
  TNode *parent;
  igraph_integer_t node_id;
  int f; // distance to d
  int size;
  int ptr;
};

struct ShortestTree_t {
  TNode *root;
  int size;
  int max_nodes;
  //false
  TNode *all_nodes;
  igraph_vector_t has_path;
  igraph_vector_t lookup;
  const igraph_t *g;
};
// igraph_vector_size

struct KShortest_t {
  STree *tree;
  Path *paths; // minheap
  int k;
  igraph_integer_t start;
};

/*
 * same node cannot appear twice in a path
 */
struct Path_t {
  int length;
  TNode *path;
};

igraph_vector_t ConstructIndexLookup (const igraph_t *g) {
  igraph_vector_t lookup;
  igraph_vector_t all_verts;
  igraph_vit_t vit;
  igraph_vs_t vs;
  igraph_integer_t max_id;
  int vcount = igraph_vcount(g);
  // igraph_vector_init(g, vcount);

  igraph_vs_all(&vs);
  igraph_vit_create(g, vs, &vit);
  igraph_vector_init(&lookup, vcount);
  // IGRAPH_CHECK(igraph_vit_create(g, vs, &vit));
  // IGRAPH_FINALLY(igraph_vit_destroy, &vit);
  int i = 0;
// while(!IGRAPH_VIT_END(vit)) {
//   long int vertex = IGRAPH_VIT_GET(vit);
//   printf("%ld\n", vertex);
//   IGRAPH_VIT_NEXT(vit);
// }

  while (!IGRAPH_VIT_END(vit)) {
    long int vertex = IGRAPH_VIT_GET(vit);
    if(vertex >= vcount) {

      igraph_vector_resize(&lookup, vertex + 1);
      vcount = vertex;
    }
    VECTOR(lookup)[ vertex ] = i;
    IGRAPH_VIT_NEXT(vit);
    i++;
  }
  return lookup;
}

STree *ConstructTree(igraph_t *g, int num_nodes, igraph_integer_t root);
TNode *ConstructTNode(igraph_integer_t id);
void DestructTree(STree *tree);
void DestructTNode(TNode *node);
void AddChild(STree *tree, igraph_integer_t start, igraph_integer_t dest);
void AddChildHelper(TNode *parent, TNode *child);
void BuildShortestTree(STree *tree);
void BuildShortestTreeHelper(STree *tree, int level, igraph_vector_t *visited, TNode *node);

STree *ConstructTree(igraph_t *g, int num_nodes, igraph_integer_t root){
  STree *tree = (STree*) malloc(sizeof(STree));
  if (tree == NULL) {
      return NULL;
  }
  if (num_nodes <= 0) {
      num_nodes = 65535;
  }
  TNode *root_node = ConstructTNode(root);
  TNode *all = (TNode *) malloc(num_nodes * sizeof(TNode));
  int i;
  TNode *tmp;
  for(i = 0; i < num_nodes; i++) {
    tmp = (all + i);
    tmp = NULL;
  }

  igraph_vector_t has_path;
  igraph_vector_init(&has_path, num_nodes);
  igraph_vector_t lookup = ConstructIndexLookup(g);
  tmp = (all + (int)VECTOR(lookup)[root]);
  tmp = root_node;
  printf("[address in constructor] %p\n", (all + (int)VECTOR(lookup)[root]));
  // TNode *root;
  // int size;
  // int max_nodes;
  // TNode *all_nodes;
  // igraph_vector_t has_path;
  tree->g = g;
  tree->root = root_node;
  tree->size = 1;
  tree->max_nodes = num_nodes;
  tree->all_nodes = all;
  tree->has_path = has_path;
  tree->lookup = lookup;
  return tree;
}

TNode *ConstructTNode(igraph_integer_t id) {
  TNode *node = (TNode*) malloc(sizeof (TNode));
  printf("[constructing node] %d\n", id);

  TNode *children = (TNode *)malloc(UNIT_LENGTH * sizeof(TNode));
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
      DestructTNode( tree->all_nodes + i );
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
  printf("[test start id : addr] %d : %p\n", start, *(tree->all_nodes + (int)VECTOR(tree->lookup)[ (int) start ]));
  // printf("[node id : addr] %d : %p\n", start, *(tree->all_nodes + (int)VECTOR(tree->lookup)[ (int) start ]));
  if(! (tree->all_nodes + (int)VECTOR(tree->lookup)[ (int) start ])) {
    printf("create new node start %d\n", start);
    TNode *tmp = (tree->all_nodes + (int)VECTOR(tree->lookup)[ (int) start ]);
    tmp = ConstructTNode((igraph_integer_t)start);
  }
  s_node = tree->all_nodes + (int) VECTOR(tree->lookup)[ (int) start ];
  d_node = tree->all_nodes + (int) VECTOR(tree->lookup)[ (int) dest ];
  AddChildHelper(d_node, s_node);
  (tree->size)++;

  VECTOR(tree->has_path)[(int) VECTOR(tree->lookup)[ (int) start ] ] = 1;
}

void AddChildHelper(TNode *parent, TNode *child) {
  if (parent->ptr == parent->size) {
    parent->children = (TNode *)realloc(parent->children, (parent->size + UNIT_LENGTH) * sizeof(TNode));
    parent->size = parent->size + UNIT_LENGTH;
  }
  parent->ptr = parent->ptr + 1;
  child->parent = parent;
  TNode *tmp = parent->children + parent->ptr;
  tmp = child;
}

void BuildShortestTree(STree *tree) {
  //  traverse through the graph to build the shortestTree
  int max_depth = 5;
  igraph_vector_t visited;
  igraph_vector_init(&visited, tree->max_nodes);
  int level = 0;

  BuildShortestTreeHelper(tree, level, &visited, tree->root);
  //
  igraph_vector_destroy(&visited);

}

void BuildShortestTreeHelper(STree *tree, int level, igraph_vector_t *visited, TNode *node) {
  // printf("[node %d] visited\n", node->node_id);
  if (VECTOR(*visited)[ (int) VECTOR(tree->lookup)[(int) node->node_id] ]) {return;}
  if (level == MAX_DEPTH) {return;}
  igraph_vector_t neighbors;
  const igraph_t *g = tree->g;
  int vcount = tree->max_nodes;

  igraph_vector_init(&neighbors, vcount);

  igraph_neighbors(g, &neighbors, node->node_id, IGRAPH_IN);
  // int i;
  // for(int i = 0; i < vc; i++) {
  //   printf("[vector id for %d] %f\n", node->node_id, VECTOR(neighbors)[i]);
  // }
  // printf("[testing] %d\n", (int)VECTOR(tree->lookup)[ (int) node->node_id ]);

  ((tree->all_nodes + (int)VECTOR(tree->lookup)[ (int) node->node_id ]))->f = level;
  int nneighbors = igraph_vector_size(&neighbors);
  int j ;
  for (j = 0; j < nneighbors; j++ ) {
    int long cur_node_id = VECTOR(neighbors)[ j ];
    if(!cur_node_id) {break;}
    // printf("[node %ld] visited\n", cur_node_id);
    if (!VECTOR(*visited)[(int) VECTOR(tree->lookup)[ (int) cur_node_id ] ]) {
      AddChild(tree, cur_node_id, node->node_id);
      VECTOR( *visited )[ (int) VECTOR(tree->lookup)[ (int) cur_node_id ] ] = 1;

      BuildShortestTreeHelper(tree, level + 1, visited, tree->all_nodes + \
                             (int)VECTOR(tree->lookup)[ (int) cur_node_id ]);
    }
  }
  igraph_vector_destroy(&neighbors);
}

void PrintTree(TNode *node, int depth) {
  if(!node) return;
  printf("[node depth]%d, [node id] %d\n", depth, node->node_id);
  int i;
  printf("[ptr size] %d\n", node->ptr);
  for(i = 0; i < node->ptr; i++) {
    PrintTree(node->children + i, depth + 1);
  }
}

int main(int argc, char const *argv[]) {
  igraph_t g;
  FILE *in_file, *out_file;
  int i, result;
  in_file = fopen(argv[1], "r");
  // out_file = fopen(argv[2], "w");
  char *pre = "pre\n";
  char *post = "post\n";
  // See if the files exist
  if (in_file==0) {
  	printf("Could not open in file\n");
  	return 1;
  }
  // if (out_file==0) {
  // 	printf("Could not open out file\n");
  // 	return 1;
  // }

  if ( (result=igraph_read_graph_graphml(&g, in_file, 0)) ) {
  	printf("Trouble reading format\n");
  	return 1;
  }
  fclose(in_file);
  int vcount = igraph_vcount(&g);
  STree *tree = ConstructTree(&g, vcount, 2);
  BuildShortestTree(tree);
  PrintTree(tree->root, 0);
  // if (out_file) {
  //   // char out = (char) tie_range;
  //
  //   char *str = malloc(5 * sizeof(char));
  //   sprintf(str,"%d", tie_range);
  //   // if (fputs(str, out_file )) {return 1;}
  //   fputs(str, out_file );
  // 	// if ( (result=igraph_write_graph_graphml(&g, out_file, 0)) ) {
  // 	// 	return 1;
  // 	// }
  //   	fclose(out_file);
  //   }
    igraph_destroy(&g);

  return 0;
}
