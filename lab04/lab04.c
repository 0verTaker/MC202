#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct _ArvoreBinaria
{
    char info;
    struct _ArvoreBinaria *esq, *dir;
} ArvoreBinaria;

char *LeCaminhoArvore(char *nomearq, int *numNo)
{
    FILE *fp = fopen(nomearq, "r");
    fscanf(fp, "%d\n", numNo);
    char *caminho = (char *)calloc((*numNo), sizeof(char));
    fscanf(fp, "%s\n", caminho);

    fclose(fp);
    return caminho;
}

ArvoreBinaria *CriaNoArvoreBinaria(ArvoreBinaria *esq, ArvoreBinaria *dir, char info)
{
    ArvoreBinaria *no = (ArvoreBinaria *)calloc(1, sizeof(ArvoreBinaria));
    no->esq = esq;
    no->dir = dir;
    no->info = info;
    return no;
}

ArvoreBinaria *ConstroiArvoreBinaria(char *pre, char *sim, int n)
{
    int ne, nd;
    ArvoreBinaria *p = NULL, *esq = NULL, *dir = NULL;

    if (n == 1)
    {
        p = CriaNoArvoreBinaria(NULL, NULL, sim[0]);
    }
    else
    {
        p = CriaNoArvoreBinaria(NULL, NULL, pre[0]);

        int raiz = 0;
        for (int i = 0; i < n; i++)
        {
            if (sim[i] == pre[0])
            {
                raiz = i;
                break;
            }
        }

        ne = raiz;
        nd = n - raiz - 1;

        if (ne > 0)
        {
            esq = ConstroiArvoreBinaria(pre + 1, sim, ne);
            p->esq = esq;
        }

        if (nd > 0)
        {
            dir = ConstroiArvoreBinaria(pre + 1 + ne, sim + 1 + ne, nd);
            p->dir = dir;
        }
    }

    return p;
}

void GrauSim(ArvoreBinaria *Arv)
{
    if (Arv != NULL)
    {
        GrauSim(Arv->esq);

        int grau = 0;
        if (Arv->esq != NULL)
            grau++;
        if (Arv->dir != NULL)
            grau++;
        printf("%c: %d\n", Arv->info, grau);

        GrauSim(Arv->dir);
    }
}

int AlturaArv(ArvoreBinaria *Arv)
{
    if (Arv != NULL)
    {
        return 1 + MAX(AlturaArv((Arv->esq)), AlturaArv(Arv->dir));
    }
    else
        return 0;
}

void DestroiArvore(ArvoreBinaria **Arv)
{
    ArvoreBinaria *p = *Arv;
    
    if (p != NULL)
    {
        DestroiArvore(&p->esq);
        DestroiArvore(&p->dir);
        free(p);
        *Arv = NULL;
    }
}

int main(int argc, char **argv)
{
    int numNoPre, numNoSim;
    ArvoreBinaria *Arv;
    
    char *pre = LeCaminhoArvore(argv[1], &numNoPre);
    char *sim = LeCaminhoArvore(argv[2], &numNoSim);

    if (numNoPre == numNoSim)
        Arv = ConstroiArvoreBinaria(pre, sim, numNoPre);
    else
        return 1;

    printf("Altura da arvore: %d\n", AlturaArv(Arv));
    printf("Grau de cada nó em percurso simétrico:\n");
    GrauSim(Arv);
    DestroiArvore(&Arv);

    return 0;
}
