#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define INF 999999

typedef struct NoGrafo
{
    char Nome;
    int x, y, NumAdj;
    double *Adj;
} NodeGrafo;

typedef struct Grafo
{
    NodeGrafo *Node;
    int NumNode;
} Grafo;

typedef struct {
    int index;
    double distance;
} HeapNode;

typedef struct {
    HeapNode *array;
    int size;
    int *pos; // Posições dos elementos no heap
} MinHeap;

typedef struct {
    int node;        // Índice do nó de origem
    double distance; // Distância mínima até o nó de destino
    int parent;      // Índice do nó predecessor no caminho mínimo
} ShortestPathResult;

Grafo *LeGrafo (char *nomearq)
{
    int NumNode, x, y;
    char Nome;

    FILE *fp = fopen(nomearq, "r");
    Grafo *G = (Grafo*)calloc(1, sizeof(Grafo));
    
    fscanf(fp, "%d\n", &NumNode);
    G->Node = (NodeGrafo*)calloc(NumNode, sizeof(NodeGrafo));
    G->NumNode = NumNode;

    for (int i = 0; i < NumNode; i++)
    {
        fscanf(fp, "%c %d %d\n", &G->Node[i].Nome, &G->Node[i].x, &G->Node[i].y);
        G->Node[i].Adj = (double*)calloc(NumNode, sizeof(double));
        
        for (int j = 0; j < NumNode; j++)
            G->Node[i].Adj[j] = 0;
    }

    return G;
}

double DistanciaEuclidiana(int x1, int y1, int x2, int y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void AchaAdj(NodeGrafo **Grafo, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                double Distancia = DistanciaEuclidiana((*Grafo)[i].x, (*Grafo)[i].y, (*Grafo)[j].x, (*Grafo)[j].y);
                if (Distancia <= 2.0)
                    (*Grafo)[i].Adj[j] = Distancia;
            }

        }
    }
}

void PrintGrafoMatriz(Grafo** Grafo, int NumNode) {
    printf("Matriz de adjacencia:\n\t");
    
    for (int i = 0; i < NumNode; i++) 
        printf("%c\t", (*Grafo)->Node[i].Nome);
    printf("\n");

    for (int i = 0; i < NumNode; i++) 
    {
        printf("%c\t", (*Grafo)->Node[i].Nome);
        for (int j = 0; j < NumNode; j++) 
        {
            if ((*Grafo)->Node[i].Adj[j] == 0)
                printf("0\t");
            else
                printf("%.3f\t", (*Grafo)->Node[i].Adj[j]);
        }
        printf("\n");
    }
}

MinHeap *createMinHeap(int capacity) {
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->array = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    return minHeap;
}

void swapHeapNode(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap *minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left].distance < minHeap->array[smallest].distance)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right].distance < minHeap->array[smallest].distance)
        smallest = right;

    if (smallest != idx) {
        HeapNode smallestNode = minHeap->array[smallest];
        HeapNode idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode.index] = idx;
        minHeap->pos[idxNode.index] = smallest;

        swapHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isInMinHeap(MinHeap *minHeap, int v) {
    if (minHeap->pos[v] < minHeap->size)
        return 1;
    return 0;
}

HeapNode extractMin(MinHeap *minHeap) {
    if (minHeap->size == 0) {
        HeapNode empty = {-1, -1};
        return empty;
    }

    HeapNode root = minHeap->array[0];
    HeapNode lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root.index] = minHeap->size - 1;
    minHeap->pos[lastNode.index] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap *minHeap, int v, double distance) {
    int i = minHeap->pos[v];
    minHeap->array[i].distance = distance;

    while (i && minHeap->array[i].distance < minHeap->array[(i - 1) / 2].distance) {
        minHeap->pos[minHeap->array[i].index] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2].index] = i;
        swapHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void printShortestPaths(Grafo *graph, ShortestPathResult *results, int V) {
    printf("Percurso mínimo:\n");

    for (int i = 0; i < V; ++i) {
        printf("Percurso [No %c]: ", graph->Node[i].Nome);

        if (results[i].distance == INF) {
            printf("E uma ilha\n");
        } else {
            // Exibe cada caminho mínimo para o nó i
            for (int j = 0; j < V; ++j) {
                    // Encontra o caminho mínimo até o nó j
                int current = j;
                printf("(%c %.6f)  ", graph->Node[current].Nome, results[current].distance);
                while (results[current].parent != -1) {
                    printf("(%c %.6f)  ", graph->Node[results[current].parent].Nome, results[results[current].parent].distance);
                    current = results[current].parent;
                }
            }
        }
        printf("\n");
    }
}

void Dijkstra(Grafo *graph, int src) {
    int V = graph->NumNode;
    ShortestPathResult *results = (ShortestPathResult *)malloc(V * sizeof(ShortestPathResult));
    MinHeap *minHeap = createMinHeap(V);

    // Inicialização dos resultados
    for (int v = 0; v < V; ++v) {
        results[v].node = v;
        results[v].distance = INF;
        results[v].parent = -1;
        minHeap->array[v].index = v;
        minHeap->array[v].distance = results[v].distance;
        minHeap->pos[v] = v;
    }

    // A distância do vértice de origem para ele mesmo é 0
    results[src].distance = 0;
    minHeap->array[src].distance = 0;
    decreaseKey(minHeap, src, results[src].distance);

    minHeap->size = V;

    while (minHeap->size > 0) {
        HeapNode heapNode = extractMin(minHeap);
        int u = heapNode.index;

        for (int i = 0; i < V; ++i) {
            double weight = graph->Node[u].Adj[i];

            if (isInMinHeap(minHeap, i) && weight && results[u].distance != INF && results[u].distance + weight <= results[i].distance) {
                results[i].distance = results[u].distance + weight;
                results[i].parent = u;
                decreaseKey(minHeap, i, results[i].distance);
            }
        }
    }

    printShortestPaths(graph, results, V);

    free(results);
}

int main(int argc, char **argv)
{
    Grafo *Grafo = LeGrafo(argv[1]);
    AchaAdj(&Grafo->Node, Grafo->NumNode);
    PrintGrafoMatriz(&Grafo, Grafo->NumNode);
    Dijkstra(Grafo, 0);
    return 0;
}