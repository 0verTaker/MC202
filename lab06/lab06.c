#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define Pai(i) ((i-1)/2)
#define FilhoEsquerdo(i) (2*i+1)
#define FilhoDireito(i) (2*i+2)

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

void Troca(NodeHeap *A, NodeHeap *B)
{
    NodeHeap aux = *A;
    *A = *B;
    *B = aux;
}

void SobeHeap(Heap **MaxHeap, int i)
{
   int pai;
   pai = Pai(i);

  while ((pai >= 0) && ((*MaxHeap)->info[pai].prioridade < (*MaxHeap)->info[i].prioridade))
   {
    Troca(&(*MaxHeap)->info[i], &(*MaxHeap)->info[pai]);
    i = pai;
    pai = Pai(i);
   }

}

void InsereHeap(Heap **MaxHeap, char *ip, int prioridade, float latencia)
{
    if (!HeapCheio(MaxHeap))
    {
        (*MaxHeap)->NumElems += 1;
        strcpy((*MaxHeap)->info[(*MaxHeap)->NumElems-1].ip, ip);
        (*MaxHeap)->info[(*MaxHeap)->NumElems-1].prioridade = prioridade;
        (*MaxHeap)->info[(*MaxHeap)->NumElems-1].latencia = latencia;
        SobeHeap(MaxHeap, (*MaxHeap)->NumElems-1);
    }
}

void LeHeap(char *nomearq, Heap **MaxHeap)
{
    int QntIp, prioridade;
    char ip[16];
    float AuxLatencia;
    
    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", &QntIp);
    
    (*MaxHeap) = CriaHeap(QntIp);
    
    for (int i = 0; i < QntIp; i++)
    {
        fscanf(fp, "%s %d %f\n ", ip, &prioridade, &AuxLatencia);
        printf("%s %d %.2f\n", ip, prioridade, AuxLatencia);
        InsereHeap(MaxHeap, ip, prioridade, AuxLatencia);
        //SobeHeap(MaxHeap, (*MaxHeap)->NumElems);
    }
    
    fclose(fp);
}

void PrintMaxHeap(Heap *MaxHeap, int indice, int nivel) {
    int i;

    if (indice < MaxHeap->NumElems) {
        PrintMaxHeap(MaxHeap, FilhoDireito(indice), nivel + 1);
        for (i = 0; i < nivel; i++)
            printf("   ");
        printf("[%d](%s)[%.2f]", MaxHeap->info[indice].prioridade, MaxHeap->info[indice].ip, MaxHeap->info[indice].latencia);
        for (i = 0; i <= (int)log2(MaxHeap->NumElems) - nivel; i++)
            printf("---");
        printf("\n");
        PrintMaxHeap(MaxHeap, FilhoEsquerdo(indice), nivel + 1);
    }
}

int main(int argc, char **argv)
{
    Heap *MaxHeap = NULL; 
    
    LeHeap("in/arq1.in", &MaxHeap);
    PrintMaxHeap(MaxHeap, 0, 1);
    
    return 0;
}
