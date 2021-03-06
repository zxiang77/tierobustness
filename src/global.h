#include <igraph.h>
/* Queue Implementation */
typedef struct data DATA;
typedef struct Node_t NODE;
typedef struct Queue Queue;
Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
NODE *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

/* ShortestTree Implementation */
typedef struct TreeNode_t TNode;
typedef struct ShortestTree_t STree;
typedef struct KShortest_t KShortest;
typedef struct Path_t Path;
STree *ConstructTree(igraph_t *g, int num_nodes, igraph_integer_t root);
TNode *ConstructTNode(igraph_integer_t id);
void DestructTree(STree *tree);
void DestructTNode(TNode *node);
void AddChild(STree *tree, igraph_integer_t start, igraph_integer_t dest);
void AddChildHelper(TNode *parent, TNode *child);
void BuildShortestTree(STree *tree);
void BuildShortestTreeHelper(STree *tree, int level, igraph_vector_t *visited, TNode *node);

/* MinHeap Implementation */
typedef struct MinHeap_t MinHeap;
