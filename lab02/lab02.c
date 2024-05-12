#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _ponto
{
    int x, y, z;
} Ponto;

typedef struct _labirinto
{
    char ***Posicao;
    int nx, ny, nz, qtd_entrada, qtd_saida;
    Ponto *entrada, *saida;
} Labirinto;

Labirinto *LeLabirinto(char *nomearq)
{
    FILE *fp = fopen(nomearq, "r");
    Labirinto *L = (Labirinto *)calloc(1, sizeof(Labirinto));

    fscanf(fp, "%d %d %d %d\n", &L->nx, &L->ny, &L->nz, &L->qtd_entrada);
    L->Posicao = (char ***)calloc(L->nz, sizeof(char **));
    L->entrada = (Ponto*)calloc(L->qtd_entrada, sizeof(Ponto));

    for (int z = 0; z < L->nz; z++)
    {
        L->Posicao[z] = (char **)calloc(L->ny, sizeof(char *));
        for (int y = 0; y < L->ny; y++)
        {
            L->Posicao[z][y] = (char *)calloc(L->nx, sizeof(char));
        }
    }

    int i = 0, num_saidas = 0;
    for (int z = 0; z < L->nz; z++)
    {
        for (int y = 0; y < L->ny; y++)
        {
            for (int x = 0; x < L->nx; x++)
            {
                fscanf(fp, " %c", &L->Posicao[z][y][x]);
                if (L->Posicao[z][y][x] == 'E')
                {
                    L->entrada[i].x = x;
                    L->entrada[i].y = y;
                    L->entrada[i].z = z;
                    i++;
                }
                else if (L->Posicao[z][y][x] == 'S')
                {
                    num_saidas++;
                    L->saida = (Ponto*)realloc(L->saida, num_saidas * sizeof(Ponto));
                    L->saida[num_saidas - 1].x = x;
                    L->saida[num_saidas - 1].y = y;
                    L->saida[num_saidas - 1].z = z;
                }
            }
        }
    }

    L->qtd_saida = num_saidas;
    fclose(fp);
    return L;
}

bool Backtrack(Labirinto *L, Ponto P)
{
    bool res = false;
    for (int i = 0; i < L->qtd_saida; i++)
    {
        if ((P.z == L->saida[i].z) && (P.y == L->saida[i].y) && (P.x == L->saida[i].x))
        {
            return true;
        }
    }
    if (!res)
    {
        Ponto *C = (Ponto *)calloc(6, sizeof(Ponto));

        C[0].x = P.x - 1; C[0].y = P.y;     C[0].z = P.z;
        C[1].x = P.x + 1; C[1].y = P.y;     C[1].z = P.z;
        C[2].x = P.x;     C[2].y = P.y - 1; C[2].z = P.z;
        C[3].x = P.x;     C[3].y = P.y + 1; C[3].z = P.z;
        C[4].x = P.x;     C[4].y = P.y;     C[4].z = P.z - 1;
        C[5].x = P.x;     C[5].y = P.y;     C[5].z = P.z + 1;
        
        char valor_original = L->Posicao[P.z][P.y][P.x];
        L->Posicao[P.z][P.y][P.x] = 'X';
        
        for (int i = 0; (i < 6) && (res != true); i++)
        {
            if ((C[i].x >= 0) && (C[i].x < L->nx) &&
                (C[i].y >= 0) && (C[i].y < L->ny) &&
                (C[i].z >= 0) && (C[i].z < L->nz))
            {
                if (L->Posicao[C[i].z][C[i].y][C[i].x] != 'X')
                {
                    res = Backtrack(L, C[i]);
                }
            }
        }
        L->Posicao[P.z][P.y][P.x] = valor_original;
        free(C);
    }

    return res;
}

void DestroiLabirinto3d(Labirinto **L)
{
    if (*L != NULL)
    {
        for (int z = 0; z < (*L)->nz; z++)
        {
            for (int y = 0; y < (*L)->ny; y++)
            {
                free((*L)->Posicao[z][y]);
            }
            free((*L)->Posicao[z]);
        }
        free((*L)->Posicao);
        free((*L)->entrada);
        free((*L)->saida);
        free(*L);
        *L = NULL;
    }
}

int main(int argc, char **argv)
{
    Labirinto *L = NULL;
    L = LeLabirinto(argv[1]);

    for (int i = 0; i < L->qtd_entrada; i++)
    {
        if (Backtrack(L, L->entrada[i]))
        {
            printf("[Entrada %d] Saída encontrada!\n", i);
        }
        else
        {
            printf("[Entrada %d] Saída não encontrada!\n", i);
        }
    }

    DestroiLabirinto3d(&L);

    return 0;
}
