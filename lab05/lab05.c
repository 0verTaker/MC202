#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct _NodeAVL{
    int prioridade, bal;
    char *ip;
    struct _NodeAVL *esq, *dir; 
}NodeAVL;

typedef struct _AVLTree{
    int size;
    NodeAVL *raiz;
}AVLTree;

NodeAVL *CriaNode(char *ip, int prioridade);
void AjusteBalancoDuplaInsercao(NodeAVL *A, NodeAVL *B, NodeAVL *C);
void AjusteBalancoSimplesInsercao(NodeAVL *A, NodeAVL *B);

void RotacaoSimplesEsquerda(NodeAVL **AVL)
{
    NodeAVL *FilhoDir = (*AVL)->dir;
    
    (*AVL)->dir = FilhoDir->esq;
    FilhoDir->esq = (*AVL);
    (*AVL) = FilhoDir;
}

void RotacaoSimplesDireita(NodeAVL **AVL)
{
    NodeAVL *FilhoEsq = (*AVL)->esq;
    
    (*AVL)->esq = FilhoEsq->dir;
    FilhoEsq->dir = (*AVL);
    (*AVL) = FilhoEsq;
}

void RotacaoDuplaEsquerda(NodeAVL **AVL)
{
    RotacaoSimplesDireita(&(*AVL)->dir);
    RotacaoSimplesEsquerda(AVL);
}

void RotacaoDuplaDireita(NodeAVL **AVL)
{
    RotacaoSimplesEsquerda(&(*AVL)->esq);
    RotacaoSimplesDireita(AVL);
}

void TrataAumentoArvoreDireita(NodeAVL **AVL, char *MaisAlta)
{
    NodeAVL *A, *B, *C;
    A = (*AVL);
    B = (*AVL)->dir;
    C = B->esq;

    switch (A->bal)
    {
        case 1:
            if (B->bal == 1) //perdeu balanço
            {
                RotacaoSimplesEsquerda(AVL);
                AjusteBalancoSimplesInsercao(A,B);
            }
            else
            {
                RotacaoDuplaEsquerda(AVL);
                AjusteBalancoDuplaInsercao(B,A,C);
            }
            *MaisAlta = 0;
            break;
        
        case 0:
            A->bal = 1;
            (*MaisAlta) = 1; 	
            break;
        
        case -1:
            A->bal = 0;
            (*MaisAlta) = 0; 	
            break;
    }
}

void TrataAumentoArvoreEsquerda(NodeAVL **AVL, char *MaisAlta)
{
    NodeAVL *A, *B, *C;
    A = (*AVL);
    B = (*AVL)->esq;
    C = B->dir;

    switch (A->bal)
    {
        case -1:
            if (B->bal == -1){
                RotacaoSimplesDireita(AVL);
                AjusteBalancoSimplesInsercao(A,B);
            }
            else
            {
                RotacaoDuplaDireita(AVL);
                AjusteBalancoDuplaInsercao(A,B,C); 
            }
            (*MaisAlta) = 0;
            break;
        case 0:
            (*AVL)->bal = -1;
            (*MaisAlta) = 1; 
            break;
        case 1:
            (*AVL)->bal = 0;
            (*MaisAlta) =  0;
            break;
    }
}

void AjusteBalancoDuplaInsercao(NodeAVL *A, NodeAVL *B, NodeAVL *C)
{
    switch (C->bal)
    {
        case -1:
        A->bal = 1;
        B->bal = 0;
        break;

        case 0:
        A->bal = 0;
        B->bal = 0;
        break;

        case +1:
        A->bal =  0;
        B->bal = -1;
        break;
    }
    C->bal = 0;
}

void AjusteBalancoSimplesInsercao(NodeAVL *A, NodeAVL *B)
{
    A->bal = 0;
    B->bal = 0;
}

void InsereNode(NodeAVL **AVL, char *ip, int prioridade, char *MaisAlta)
{
    if ((*AVL) == NULL)
    {
        (*AVL) = CriaNode(ip, prioridade);
        *MaisAlta = 1;
    }
    else
    {
        if ((*AVL)->prioridade <= prioridade)
        {
            InsereNode(&((*AVL)->dir), ip, prioridade, MaisAlta);
            if (*MaisAlta == 1)
                TrataAumentoArvoreDireita(AVL, MaisAlta);
        }
        else
        {
            InsereNode(&((*AVL)->esq), ip, prioridade, MaisAlta);
            if (*MaisAlta == 1)
                TrataAumentoArvoreEsquerda(AVL, MaisAlta);
        }
    }
}

void PrintConstruida(NodeAVL *AVL)
{
    if (AVL != NULL)
    {
        printf("- %s: %d\n", AVL->ip, AVL->prioridade);
        PrintConstruida(AVL->esq);
        PrintConstruida(AVL->dir);
    }
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
    
    Node->ip = strdup(ip);
    Node->prioridade = prioridade;
    Node->bal = 0;
    Node->dir = Node->esq = NULL;

    return Node;
}

void LeAVL(char *nomearq, AVLTree **AVL, char *MaisAlta)
{
    int SizeAux, prioridade;
    char ip[16];
    
    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", &SizeAux);
    
    (*AVL) = CriaAVL(SizeAux);

    for (int i = 0; i < SizeAux; i++)
    {
        fscanf(fp, "%s %d\n ", ip, &prioridade);
        //NodeAVL *NewNode = CriaNode(ip, prioridade);
        InsereNode(&(*AVL)->raiz, ip, prioridade, MaisAlta);
    }

    fclose(fp);
}

int AlturaAVL(NodeAVL *AVL)
{
    if (AVL != NULL)
    {
        return 1 + MAX(AlturaAVL((AVL->esq)), AlturaAVL(AVL->dir));
    }
    else
        return 0;
}

bool AVLCheia(AVLTree *AVL)
{
    int Altura = AlturaAVL(AVL->raiz);
    int MaxNodeCount = pow(2, Altura+1) - 1;
    
    return (AVL->size == MaxNodeCount);
}

int MaiorRota(NodeAVL *AVL)
{
    return (AlturaAVL(AVL->esq) + AlturaAVL(AVL->dir)) + 1;
}

int main(int argc, char **argv)
{
    char MaisAlta;
    AVLTree *AVL = NULL;
    
    LeAVL(argv[1], &AVL, &MaisAlta);
    
    printf("[INFO] Apos construcao:\n");
    PrintConstruida(AVL->raiz);
    
    if (AVLCheia(AVL))
        printf("Arvore esta cheia\n");
    else
        printf("Arvore nao esta cheia\n");

    printf("A rota mais longa possível passa por %d nos", MaiorRota(AVL->raiz));

    //LeComandos(argv[2]);
}