#include <stdio.h>
#include <stdlib.h>

/* Algoritmo para desenhar linhas em uma matriz e verificar linhas que
se interceptam. -- variáveis compostas e homogêneas. */
/*
Cabeçalho: Implemente um programa que:

- Receba os parâmetros de duas retas (x1 y1 x2 y2) (x3 y3 x4 y4)
- Desenhe essas duas linhas em uma matriz de [100, 100]
- Verifique se elas se interceptam, e caso se interceptem, exiba o ponto de interseção (Dentro da Matriz).
- Por fim, exiba a matriz, utilizando a função Printmatriz
*/

typedef struct ponto
{
    int x, y;
} Ponto;
// y = mx + n
typedef struct reta
{
    Ponto inicio, fim;
    float coeficiente_angular; // m
    float coeficiente_linear;  // n
} Reta;

int **CriaMatriz(int altura, int largura);
void DestroiMatriz(int **matriz, int altura);
void ExibeMatriz(int **matriz, int altura, int largura);
void DesenhaLinhas(int **matriz, int altura, int largura, Reta reta);
void ConfiguraCoeficientesReta(Reta *reta);
/* Retorna:
0: Se paralelas
1: Se concorrentes
2: Se coincidentes
*/
int VerificaInterseccao(Reta reta_1, Reta reta_2, Ponto *interseccao);

int main(int argc, char *argv[])
{
    Reta reta_1, reta_2;

    scanf("(%d %d %d %d) (%d %d %d %d)", &reta_1.inicio.x, &reta_1.inicio.y, &reta_1.fim.x, &reta_1.fim.y, &reta_2.inicio.x, &reta_2.inicio.y, &reta_2.fim.x, &reta_2.fim.y);

    int largura = 100, altura = 100;
    int **matriz = CriaMatriz(altura, largura);
    ConfiguraCoeficientesReta(&reta_1);
    ConfiguraCoeficientesReta(&reta_2);
    DesenhaLinhas(matriz, altura, largura, reta_1);
    DesenhaLinhas(matriz, altura, largura, reta_2);
    Ponto interseccao;
    int status = VerificaInterseccao(reta_1, reta_2, &interseccao);
    if (status == 0)
    {
        printf("Retas são paralelas\n");
    }
    else if (status == 1)
    {
        printf("Retas são concorrentes\n");
        if (
            (interseccao.x > 0 && interseccao.x < largura) &&
            (interseccao.y > 0 && interseccao.y < altura))
        {
            printf("Intersecção ocorre na matriz\n");
            // Marca o ponto de intersecção com 2
            matriz[99 - interseccao.y][interseccao.x] = 2;
        }
        else
        {
            printf("Intersecção ocorre fora da matriz\n");
        }
        printf("Intersecção: (%d, %d)\n", interseccao.x, interseccao.y);
    }
    else if (status == 2)
    {
        printf("Retas são coincidentes\n");
    }

    ExibeMatriz(matriz, altura, largura);
    DestroiMatriz(matriz, altura);

    return 0;
}

int VerificaInterseccao(Reta reta_1, Reta reta_2, Ponto *interseccao)
{
    /*
    TODO:
    - Seu código recebe duas retas
    - Através da análise do coeficiente angular e linear, verifique se as retas são:
    - Coincidentes
    - Paralelas
    - Concorrentes
    - Caso as retas sejam concorrentes, calcule o ponto de intersecção
    - Lembre-se de converter seu ponto de intersecção para int: (int) value
    - Seu código deve retornar
    - 0: Se retas paralelas
    - 1: Se retas concorrentes
    - 2: Se retas coincidentes
    */

    float sample = reta_1.coeficiente_angular - reta_2.coeficiente_angular;

    if ((reta_1.inicio.x - reta_1.fim.x) == 0 || (reta_2.inicio.x - reta_2.fim.x) == 0)
    {
        if ((reta_1.inicio.x - reta_1.fim.x) == 0 && (reta_2.inicio.x - reta_2.fim.x) == 0)
        {
            if (reta_1.inicio.x == reta_2.inicio.x)
                return 2; // retas coincidentes
            else
                return 0; // retas paralelas
        }

        else
        {
            float x = 0, y = 0;
            if (reta_1.coeficiente_angular == 0)
            {
                x = (reta_2.coeficiente_linear - reta_1.inicio.x) / sample;
                y = reta_1.coeficiente_angular * x + reta_1.coeficiente_linear;
            }
            else
            {
                x = (reta_2.inicio.x - reta_1.coeficiente_linear) / sample;
                y = reta_1.coeficiente_angular * x + reta_1.coeficiente_linear;
            }
            interseccao->x = (int)x;
            interseccao->y = (int)y;

            return 1;
        }
    }

    else
    {
        if (reta_1.coeficiente_angular == reta_2.coeficiente_angular)
        {
            if (reta_1.coeficiente_linear == reta_2.coeficiente_linear)
                return 2; // retas coincidentes
            else
                return 0; // retas paralelas
        }
        else
        {
            // retas concorrentes
            float x = (reta_2.coeficiente_linear - reta_1.coeficiente_linear) / sample;
            float y = reta_1.coeficiente_angular * x + reta_1.coeficiente_linear;

            interseccao->x = (int)x;
            interseccao->y = (int)y;

            return 1;
        }
    }
}

void DesenhaLinhas(int **matriz, int altura, int largura, Reta reta)
{
    /*
    TODO:
    - Aqui o objetivo é utilizar comandos de desvio e repetição
    - Desvio = Comando if, else, e elseif
    - Repetição = Comando for
    - Com os parâmetros da reta calculados, podemos iterar pelos valores inteiros
    de x e calcular y, para por fim desenhar na matriz
    - Não se esqueça de inverter o eixo y
    - Lide com o caso onde o coeficiente angular não existe (Não cruza o eixo y)
    - Também verifique se você não esta utilizando pontos fora da matriz
    */
    // verifica se a reta é vertical
    if (reta.inicio.x == reta.fim.x)
    {
        // reta vertical
        for (int y = reta.inicio.y; y < reta.fim.y; y++)
        {
            if (y >= 0 && y < altura)
            {
                matriz[altura - y - 1][reta.inicio.x] = 1;
            }
        }
    }
    else
    {
        // coeficiente angular != 0
        for (int x = reta.inicio.x; x <= reta.fim.x; x++)
        {
            // calcula o valor de y para o ponto atual de x
            int y = reta.coeficiente_angular * x + reta.coeficiente_linear;

            if (x >= 0 && x < largura && y >= 0 && y < altura)
            {
                matriz[altura - y - 1][x] = 1;
            }
        }
    }
}

void ConfiguraCoeficientesReta(Reta *reta)
{
    /*
    TODO:
    - Aqui o objetivo é utilizar comandos de desvio
    - Desvio = Comando if, else, e elseif
    - Sabendo os valores de inicio e fim da sua reta, calcule o coeficiente angular
    dela
    - Verifique se o coeficiente angular é 0 (reta paralela ao eixo x)
    - Lembre-se de converter seus valores para float antes de operações matemáticas,
    pois int / int = int
    - Conversão (float)valor
    */
    reta->coeficiente_angular = 0;
    reta->coeficiente_linear = 0;
    float reta_fim_x = reta->fim.x;
    float reta_ini_y = reta->inicio.y;
    float reta_ini_x = reta->inicio.x;
    float reta_fim_y = reta->fim.y;

    // verifica divisão por 0
    if (reta_fim_x - reta_ini_x != 0)
    {
        reta->coeficiente_angular = (reta_fim_y - reta_ini_y) / (reta_fim_x - reta_ini_x);
        reta->coeficiente_linear = reta_ini_y - (reta->coeficiente_angular * reta_ini_x);
    }
    else
    {
        // quando o coeficiente angular é 0, o coeficiente linear é a coordenada y do ponto inicial
        reta->coeficiente_linear = reta_ini_y;
    }
}

void ExibeMatriz(int **matriz, int altura, int largura)
{
    for (int linha = 0; linha < altura; linha++)
    {
        printf("[ ");
        for (int coluna = 0; coluna < largura; coluna++)
        {
            printf("%d ", matriz[linha][coluna]);
        }
        printf("]\n");
    }
}

int **CriaMatriz(int altura, int largura)
{
    int **matriz = (int **)calloc(altura, sizeof(int *));
    // Para cada linha, aloca memória para suas colunas
    for (int linha = 0; linha < altura; linha++)
    {
        matriz[linha] = (int *)calloc(largura, sizeof(int));
        // Inicializa os valores da linha
        for (int coluna = 0; coluna < largura; coluna++)
        {
            matriz[linha][coluna] = 0;
        }
    }

    return matriz;
}

void DestroiMatriz(int **matriz, int altura)
{
    if (matriz != NULL)
    {
        for (int i = 0; i < altura; i++)
        {
            if (matriz[i] != NULL)
            {
                free(matriz[i]);
            }
        }
        free(matriz);
    }
}