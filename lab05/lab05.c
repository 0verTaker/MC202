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

void LeAVL(char *nomearq, AVLTree **AVL)
{
    int SizeAux, prioridade;
    char *ip[100];
    
    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", &SizeAux);
    
    (*AVL) = CriaAVL(SizeAux);

    for (int i = 0; i < SizeAux; i++)
    {
        fscanf(fp, "%s %d\n", ip, &prioridade);
        NodeAVL *NewNode = CriaNode(ip, prioridade);
        InsereNode(*AVL, NewNode);
    }

    fclose(fp);
}


AVLTree *CriaAVL(int size)
{
    AVLTree *p = (AVLTree*)calloc(1,sizeof(AVLTree));
    p->size = size;
    p->raiz = NULL;

    return p;
}

NodeAVL *CriaNode(char *ip, int prioridade)
{
    NodeAVL *Node = (NodeAVL*)calloc(1, sizeof(NodeAVL));
    Node->ip = ip;
    Node->prioridade = prioridade;
    Node->dir = Node->esq = NULL;

    return Node;
}

int main(int argc, char **argv)
{
    AVLTree *AVL = NULL;
}