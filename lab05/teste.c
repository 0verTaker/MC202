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
void AjusteBalancoRotacaoEsquerdaSimplesRemocao(NodeAVL *A, NodeAVL *B, char *MaisBaixa);
void AjusteBalancoRotacaoDireitaSimplesRemocao(NodeAVL *A, NodeAVL *B, char *MaisBaixa);
void AjusteBalancoRotacaoEsquerdaDuplaRemocao(NodeAVL *A, NodeAVL *B, NodeAVL *C);
void AjusteBalancoRotacaoDireitaDuplaRemocao(NodeAVL *A, NodeAVL *B, NodeAVL *C);
void PrintConstruida(NodeAVL *AVL);

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

void TrataReducaoArvoreDireita(NodeAVL **AVL, char *MaisBaixa)
{
    NodeAVL *A,*B,*C;
    A = (*AVL); 
    B = (*AVL)->esq;

    switch(A->bal)
    {
        case -1:
            if (B->bal <= 0)
            {
                RotacaoSimplesDireita(AVL);
                AjusteBalancoRotacaoDireitaSimplesRemocao(A, B, MaisBaixa); 
            }
            else
            {
                C = B->dir;
                RotacaoDuplaDireita(AVL);
                AjusteBalancoRotacaoDireitaDuplaRemocao(A, B, C);
                (*MaisBaixa) =  1;
            }
            break;
        
        case 0:
            (*AVL)->bal = -1;
            (*AVL) = 0;
            break; 

        case 1:
            (*AVL)->bal = 0;
            (*MaisBaixa) = 1;
            break;   
    }
}

void TrataReducaoArvoreEsquerda(NodeAVL **AVL, char *MaisBaixa)
{
    NodeAVL *A,*B,*C;
    
  A = (*AVL); 
  B = (*AVL)->dir;

    switch(A->bal)
    {
        case 1:
            if(B->bal >= 0)
            {
                RotacaoSimplesEsquerda(AVL);
                AjusteBalancoRotacaoEsquerdaSimplesRemocao(A, B, MaisBaixa);
            }
            else
            {
                C = B->esq;
                RotacaoDuplaEsquerda(AVL);
                AjusteBalancoRotacaoEsquerdaDuplaRemocao(A, B, C);
                (*MaisBaixa) =  1;
            }
            break;
        
        case 0:
            A->bal = 1;    
            (*MaisBaixa) = 0;
            break;

        case -1:
            A->bal = 0;  
            (*MaisBaixa) =  1;
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

void AjusteBalancoRotacaoDireitaDuplaRemocao(NodeAVL *A, NodeAVL *B, NodeAVL *C)
{
    switch(C->bal) {
        case -1:
            A->bal = 1;
            B->bal = 0;
            break;
        
        case 0:
            A->bal =  0;
            B->bal =  0;
            break;
        
        case +1:
            A->bal =  0;
            B->bal = -1;
            break;
        }
        C->bal = 0;
}

void AjusteBalancoRotacaoEsquerdaDuplaRemocao(NodeAVL *A, NodeAVL *B, NodeAVL *C)
{
    switch(C->bal) 
    {
        case -1:
            A->bal = 0;
            B->bal = 1;
            break;
        
        case 0:
            A->bal = 0;
            B->bal = 0;
            break;
        
        case +1:
            A->bal = -1;
            B->bal =  0;
            break;
    }
        C->bal = 0;
}

void AjusteBalancoRotacaoDireitaSimplesRemocao(NodeAVL *A, NodeAVL *B, char *MaisBaixa)
{
    if (B->bal == -1)
    { 
        A->bal =  0;
        B->bal =  0;
        *MaisBaixa = 1;
    } 
    else 
    {
        A->bal = -1;
        B->bal =  1;
        *MaisBaixa = 0;
    }
}

void AjusteBalancoRotacaoEsquerdaSimplesRemocao(NodeAVL *A, NodeAVL *B, char *MaisBaixa)
{
    if (B->bal == 1)
    { 
        A->bal =  0;
        B->bal =  0;
        *MaisBaixa = 1;
    } 
    else 
    { 
        A->bal =  1;
        B->bal = -1;
        *MaisBaixa = 0;
    }
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

char RemoveNoGrau0ou1(NodeAVL **AVL, char *MaisBaixa)
{
    NodeAVL *aux;

    if ((*AVL)->esq == NULL)
    {
        aux = (*AVL);
        (*AVL) = (*AVL)->dir;
        free(aux);
    }
    else
    {
        if ((*AVL)->dir == NULL)
        {
            aux = (*AVL);
            (*AVL) = (*AVL)->esq;
            free(aux);
        }
        else
            return 0;
    }
    (*MaisBaixa) = 1;

    return 1;
}

void SubstituiRemoveMenorSucessor(NodeAVL **AVL, NodeAVL **MaisEsq, char *MaisBaixa)
{
    if ((*MaisEsq)->esq == NULL)
    {
        (*AVL)->ip = (*MaisEsq)->ip;
        (*AVL)->prioridade = (*MaisEsq)->prioridade;
        
        RemoveNoGrau0ou1(MaisEsq, MaisBaixa); 
    }
    else
    {
        SubstituiRemoveMenorSucessor(AVL, &((*MaisEsq)->esq), MaisBaixa);

        if ((*MaisBaixa))
            TrataReducaoArvoreEsquerda(MaisEsq, MaisBaixa);
    }
}

void RemoveDeFato(NodeAVL **AVL, char *MaisBaixa)
{
    int bal;

    if (RemoveNoGrau0ou1(AVL, MaisBaixa) == 0)
    {
        bal = (*AVL)->bal;
        SubstituiRemoveMenorSucessor(AVL, &((*AVL)->dir), MaisBaixa);
        (*AVL)->bal = bal;

        if ((*MaisBaixa))
            TrataReducaoArvoreDireita(AVL, MaisBaixa);
    }
}

void RemoveNode(NodeAVL **AVL, char *ip, int prioridade, char *MaisBaixa)
{
    if ((*AVL) != NULL)
    {
        if ((*AVL)->prioridade != prioridade && strcmp((*AVL)->ip, ip) != 0)
            {
            if ((*AVL)->prioridade < prioridade)
            {
                RemoveNode(&((*AVL)->dir), ip, prioridade, MaisBaixa);
                if ((*MaisBaixa == 1))
                    TrataReducaoArvoreDireita(AVL, MaisBaixa);
            }
            else
            {
                RemoveNode(&((*AVL)->esq), ip, prioridade, MaisBaixa);
                if ((*MaisBaixa == 1))
                    TrataReducaoArvoreEsquerda(AVL, MaisBaixa);
            }
        }
        else
            RemoveDeFato(AVL, MaisBaixa);
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

void LeComandos(char *nomearq, AVLTree **AVL, char *MaisAlta, char *MaisBaixa)
{
    int QntCmd, prioridade, cmd;
    char ip[16];

    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", &QntCmd);

    for (int i = 0; i < QntCmd; i++)
    {
        fscanf(fp, "%d %s %d\n ", &cmd, ip, &prioridade);

        //printf("cmd: %d, ip: %s, p: %d\n", cmd, ip, prioridade);
        if (cmd == 1)
            InsereNode(&(*AVL)->raiz, ip, prioridade, MaisAlta);
            //printf("1\n");
        else
        {
            RemoveNode(&(*AVL)->raiz, ip, prioridade, MaisBaixa);
            //printf("-1\n");
        }

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

void PrintConstruida(NodeAVL *AVL)
{
    if (AVL != NULL)
    {
        printf("- %s: %d\n", AVL->ip, AVL->prioridade);
        PrintConstruida(AVL->esq);
        PrintConstruida(AVL->dir);
    }
}

int main(int argc, char **argv)
{
    char MaisAlta = 0, MaisBaixa = 0;
    AVLTree *AVL = NULL;
    
    LeAVL("in/arq1.in1", &AVL, &MaisAlta);
    
    printf("[INFO] Apos construcao:\n");
    PrintConstruida(AVL->raiz);
    
    if (AVLCheia(AVL))
        printf("Arvore esta cheia\n");
    else
        printf("Arvore nao esta cheia\n");

    printf("A rota mais longa possível passa por %d nos\n", MaiorRota(AVL->raiz));

    LeComandos("in/arq1.in2", &AVL, &MaisAlta, &MaisBaixa);

    printf("\n[INFO] Apos atualizacao:\n");
    PrintConstruida(AVL->raiz);

    if (AVLCheia(AVL))
        printf("Arvore esta cheia\n");
    else
        printf("Arvore nao esta cheia\n");

    //printf("A rota mais longa possível passa por %d nos\n", MaiorRota(AVL->raiz));

}

