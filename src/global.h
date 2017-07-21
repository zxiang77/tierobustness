typedef struct data DATA;
typedef struct Node_t NODE;
typedef struct Queue Queue;
Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
NODE *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

typedef struct TreeNode_t TNode;
typedef struct ShortestTree STree;
STree *ConstructTree(int num_nodes);
TNode *ConstructTNode(igraph_integer_t id);
void DestructTree(STree *tree);
void DestructTNode(TNode *node);
int AddChild(TNode *parent, TNode *child);
