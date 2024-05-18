#include <stdio.h>
#include <stdlib.h>

typedef struct _NodeAVL{
    int prioridade;
    char *ip;
    struct _NodeAVL *esq, *dir; 
}NodeAVL;

typedef struct _AVLTree{
    int size;
    NodeAVL *raiz;
}AVLTree;

void LeArquivo(char *nomearq, AVLTree **AVL)
{
    int SizeAux;
    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", &SizeAux);
    (*AVL) = CriaAVL(SizeAux);
    
}


AVLTree *CriaAVL(int size)
{
    AVLTree *p = (AVLTree*)calloc(1,sizeof(AVLTree));
    p->size = size;
    p->raiz = NULL;

    return p;
}

void CriaNode(AVLTree *AVL, char *ip, int prioridade)
{
    NodeAVL *Node = (NodeAVL*)calloc(1, sizeof(NodeAVL));
    Node->ip = ip;
    Node->prioridade = prioridade;
    Node->dir = Node->esq = NULL;

    AVL->raiz = Node;
}

int main(int argc, char **argv)
{
    AVLTree *AVL = NULL;
}