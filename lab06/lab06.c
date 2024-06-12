#include <stdio.h>
#include <stdlib.h>
#include <stbool.h>
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
    int MaxSize, NumElem;
} Heap;

Heap *CriaHeap(int MaxSize)
{
    Heap *H = (Heap*)calloc(1, sizeof(Heap));

    H->MaxSize = MaxSize;
    H->NumElem = 0;
    H->info = (NodeHeap*)calloc(H->MaxSize, sizeof(NodeHeap));

    return H;
}

bool HeapCheio(Heap **MaxHeap)
{
    if ((*MaxHeap)->NumElem >= (*MaxHeap)->MaxSize)
        return true;
    else
        return false;
}

void InsereHeap(Heap **MaxHeap, char *ip, int prioridade, float, latencia)
{
    if (!HeapCheio(MaxHeap))
    {
        (*MaxHeap)->NumElem += 1;
        strcpy((*MaxHeap)->info[(*MaxHeap)->nelems-1].ip, ip);
        (*MaxHeap)->info[(*MaxHeap)->nelems-1].prioridade = prioridade;
        (*MaxHeap)->info[(*MaxHeap)->nelems-1].latencia = latencia;
    }
}

void LeHeap(char *nomearq, Heap **MaxHeap)
{
    int QntIp;
    char ip[16];
    float AuxLatencia;
    
    FILE *fp = fopen(nomearq, "r");
    fsacnf(fp, %d\n, &QntIp);
    
    (*MaxHeap) = CriaHeap(QntIp);
    
    for (int i = 0; i < QntIp; i++)
    {
        fscanf(fp, "%s %d %f\n ", ip, &prioridade, &AuxLatencia);
        InsereHeap(&(*MaxHeap)->info, ip, prioridade, AuxLatencia);
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
    Heap *MaxHeap = NULL; 
    LeHeap(argv[1], &MaxHeap);
    
    return 0;
}
