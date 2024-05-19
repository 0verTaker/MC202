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

NodeAVL *InsereNode(NodeAVL *AVL, char *ip, int prioridade);
int Altura(NodeAVL *AVL);

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
    Node->bal = 1;
    Node->dir = Node->esq = NULL;

    return Node;
}

AVLTree *LeAVL(char *nomearq, AVLTree *AVL)
{
    int SizeAux, prioridade;
    char ip[16];
    
    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", &SizeAux);
    
    AVL = CriaAVL(SizeAux);

    for (int i = 0; i < SizeAux; i++)
    {
        fscanf(fp, "%s %d\n ", ip, &prioridade);
        //NodeAVL *NewNode = CriaNode(ip, prioridade);
        AVL->raiz = InsereNode(AVL->raiz, ip, prioridade);
    }
    fclose(fp);

    return AVL;
}

void LeComandos(char *nomearq, AVLTree *AVL, char *MaisAlta, char *MaisBaixa)
{
    int QntCmd, prioridade, cmd;
    char ip[20];

    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", &QntCmd);

    for (int i = 0; i < QntCmd; i++)
    {
        fscanf(fp, "%d %s %d\n ", &cmd, ip, &prioridade);

        //printf("cmd: %d, ip: %s, p: %d\n", cmd, ip, prioridade);
        if (cmd == 1)
            InsereNode(AVL->raiz, ip, prioridade);
            //printf("1\n");
        else
        {
            // RemoveNode(&(*AVL)->raiz, ip, prioridade);
            //printf("-1\n");
        }

    }

    fclose(fp);
}

int MaiorRota(NodeAVL *AVL)
{
    return (Altura(AVL->esq) + Altura(AVL->dir)) + 1;
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

int Altura(NodeAVL *AVL)
{
    if (AVL == NULL)
        return 0;
    return AVL->bal;
}

NodeAVL *RotacaoDireita(NodeAVL *Y)
{
    NodeAVL *X = Y->esq;
    NodeAVL *T2 = X->dir;

    X->dir = Y;
    Y->esq = T2;

    Y->bal = MAX(Altura(Y->esq), Altura(Y->dir)) + 1;
    X->bal = MAX(Altura(X->esq), Altura(X->dir)) + 1;

    return X;
}

NodeAVL *RotacaoEsquerda(NodeAVL *X)
{
    NodeAVL *Y = X->dir;
    NodeAVL *T2 = Y->esq;

    Y->esq = X;
    X->dir = T2;

    X->bal = MAX(Altura(X->esq), Altura(X->dir)) + 1;
    Y->bal = MAX(Altura(Y->esq), Altura(Y->dir)) + 1;

    return Y;
}

int PegaBalanco(NodeAVL *AVL)
{
    if (AVL == NULL)
        return 0;
    return Altura(AVL->esq) - Altura(AVL->dir);
}

NodeAVL *InsereNode(NodeAVL *AVL, char *ip, int prioridade)
{
    if (AVL == NULL)
        return CriaNode(ip, prioridade);
    else
    {
        if (prioridade <= AVL->prioridade)
        {
          AVL->esq = InsereNode(AVL->esq, ip, prioridade);
        }
        else if (prioridade >= AVL->prioridade)
        {
            AVL->dir = InsereNode(AVL->dir, ip, prioridade);
        }
        else
            return AVL;
    }

    AVL->bal = 1 + MAX(Altura(AVL->esq), Altura(AVL->dir));

    int balanco = PegaBalanco(AVL); 

    if (balanco > 1 && prioridade <= AVL->esq->prioridade)
        return RotacaoDireita(AVL);

    if (balanco < -1 && prioridade >= AVL->dir->prioridade)
        return RotacaoEsquerda(AVL);
    
    if (balanco > 1 && prioridade >= AVL->esq->prioridade)
    {
        AVL->esq = RotacaoEsquerda(AVL->esq);
        return RotacaoDireita(AVL);
    }

    if (balanco < -1 && prioridade <= AVL->dir->prioridade)
    {
        AVL->dir = RotacaoDireita(AVL->dir);
        return RotacaoEsquerda(AVL);
    }
    
    return AVL;
}

int main(int argc, char **argv)
{
    AVLTree *AVL = NULL;
    
    AVL = LeAVL("in/arq3.in1", AVL);
    
    printf("[INFO] Apos construcao:\n");
    PrintConstruida(AVL->raiz);
    
    if (AVLCheia(AVL))
        printf("Arvore esta cheia\n");
    else
        printf("Arvore nao esta cheia\n");

    printf("A rota mais longa possível passa por %d nos\n", MaiorRota(AVL->raiz));

    //LeComandos("in/arq1.in2", &AVL, &MaisAlta, &MaisBaixa);

    printf("\n[INFO] Apos atualizacao:\n");
    // PrintConstruida(AVL->raiz);

    // if (AVLCheia(AVL))
    //     printf("Arvore esta cheia\n");
    // else
    //     printf("Arvore nao esta cheia\n");

    //printf("A rota mais longa possível passa por %d nos\n", MaiorRota(AVL->raiz));

}