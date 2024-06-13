#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct _NodeHeap
{
    char ip[16];
    int prioridade;
    float latencia;
} NodeHeap;

typedef struct _heap
{
    NodeHeap *info;   
    int MaxSize, NumElems;
} Heap;

Heap *CriaHeap(int MaxSize)
{
    Heap *H = (Heap*)calloc(1, sizeof(Heap));

    H->MaxSize = MaxSize;
    H->NumElems = 0;
    H->info = (NodeHeap*)calloc(H->MaxSize, sizeof(NodeHeap));

    return H;
}

bool HeapCheio(Heap **MaxHeap)
{
    if ((*MaxHeap)->NumElems >= (*MaxHeap)->MaxSize)
        return true;
    else
        return false;
}

void InsereHeap(Heap **MaxHeap, char *ip, int prioridade, float latencia)
{
    if (!HeapCheio(MaxHeap))
    {
        (*MaxHeap)->NumElems += 1;
        strcpy((*MaxHeap)->info[(*MaxHeap)->NumElems-1].ip, ip);
        (*MaxHeap)->info[(*MaxHeap)->NumElems-1].prioridade = prioridade;
        (*MaxHeap)->info[(*MaxHeap)->NumElems-1].latencia = latencia;
    }
}

void Sobeheap(Heap **MAxHeap)
{
    
}

void LeHeap(char *nomearq, Heap **MaxHeap)
{
    int QntIp, prioridade;
    char ip[16];
    float AuxLatencia;
    
    FILE *fp = fopen(nomearq, "r");
    fsacnf(fp, "%d\n", &QntIp);
    
    (*MaxHeap) = CriaHeap(QntIp);
    
    for (int i = 0; i < QntIp; i++)
    {
        fscanf(fp, "%s %d %f\n ", ip, &prioridade, &AuxLatencia);
        InsereHeap(&(*MaxHeap)->info, ip, prioridade, AuxLatencia);
        SobeHeap(MaxHeap);
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
    Heap *MaxHeap = NULL; 
    LeHeap(argv[1], &MaxHeap);
    
    return 0;
}
