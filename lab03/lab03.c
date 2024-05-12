#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/*
Esse programa define uma imagem 2D binária com valores 0 e 1 como um array multidimensional 2D (ou matriz) representado por uma struct, cujos elementos da matriz são alocados como nós conectados sequencialmente
na Lista Ligada. O principal objetivo do programa é realizar operações sobre os valores desta imagem considerando seus elementos vizinhos ou adjacentes. A operação de Dilatação considera
o máximo nas comparações entre os valores localizados (ou até) na borda do desenho na matriz com seus valores adajcentes, enquanto a operação de Erosão considera o mínimo.
A combinação das operações de Dilatação e Erosão resultam nas outras operações chamadas Abertura e Fechamento.
*/

FILE *arquivo_saida_ponteiro = NULL;

typedef struct no_lista_simples
{
    int valor;
    struct no_lista_simples *proximo;
} ListaSimples, NoListaSimples;

typedef struct _pixel
{
    int x;
    int y;
} Pixel;

typedef struct _imagem
{
    int **Posicao;
    int largura;
    int altura;
} Imagem;

// Definindo e inicializando uma variável nula do tipo no_lista_simples que vai auxiliar
// nas operações sobre a estrutura de dados Lista Ligada
ListaSimples *noAtual = NULL;

// Declaração de algumas funções (Opcional)
bool ListaSimplesVazia(ListaSimples *inicio);
bool BuscaElementoListaSimples(ListaSimples *inicio, int elem, NoListaSimples **pos, NoListaSimples **ant);
void InserirElementoNoFimLista(ListaSimples **inicio, int valor);
bool RemoveInicioListaSimples(ListaSimples **inicio, int *elem);
void DestroiListaLigada(ListaSimples **inicio);
NoListaSimples *ObterNo(NoListaSimples *noAtual, int indice);
void Dilatacao(NoListaSimples **noAtual, int n, int m);
void Erosao(NoListaSimples **noAtual, int n, int m);
void Abertura(NoListaSimples **noAtual, int n, int m);
void Fechamento(NoListaSimples **noAtual, int n, int m);

// Função para mostrar erro no console e sair do programa
void Erro(char *msg, char *funcao, ...)
{
    va_list args; /* lista de argumentos */
    char msg_final[4096];

    va_start(args, funcao);         /* inicializa a lista args para recuperar
                           argumentos após o argumento funcao */
    vsprintf(msg_final, msg, args); /* envia a lista inteira de
                       argumentos em args para
                       formatação em msg_final */
    va_end(args);                   /* finaliza o processo de recuperação */

    printf("Erro em %s: %s\n", funcao, msg_final);
    fflush(stdout);
    exit(1);
}

// Função para ler imagem binária de arquivo e usar elementos obtidos na alocação dinâmica da struct apropriada
Imagem *LeImagem(char const *nomearquivo)
{
    FILE *arquivo_ponteiro = fopen(nomearquivo, "r");
    if (arquivo_ponteiro == NULL)
    {
        Erro("Erro na abertura do arquivo\n", "LeImagem", arquivo_ponteiro);
        return NULL;
    }
    Imagem *img = (Imagem *)calloc(1, sizeof(Imagem));

    fscanf(arquivo_ponteiro, "%d %d\n", &img->largura, &img->altura);

    img->Posicao = (int **)calloc(img->altura, sizeof(int *));
    for (int y = 0; y < img->altura; y++)
        img->Posicao[y] = (int *)calloc(img->largura, sizeof(int));

    for (int y = 0; y < img->altura; y++)
    {
        for (int x = 0; x < img->largura; x++)
        {
            char caractere_atual;
            fscanf(arquivo_ponteiro, "%c ", &caractere_atual);

            // precisamos converter os caracteres para um inteiros, então subtraimos por caractere '0' da tabela ASCII
            // Isso funciona porque o valor ASCII dos caracteres ‘0’ a ‘9’ são consecutivos.
            // Logo, subtraindo o valor ASCII de ‘0’ do valor ASCII de qualquer digito caractere deve resultar no equivalente inteiro do digito. Por exemplo,
            // o valor ASCII de ‘1’ é 49 e o valor ASCII de ‘0’ é 48, então 49 - 48 = 1, que é o equivalente inteiro do caractere ‘1’.
            img->Posicao[y][x] = (int)(caractere_atual - '0');
        }
        fscanf(arquivo_ponteiro, "\n");
    }

    fclose(arquivo_ponteiro);
    return (img);
}

// Função para verificar se Lista Ligada Simples possui Nó
bool ListaSimplesVazia(ListaSimples *inicio)
{
    if (inicio == NULL)
        return true;
    else
        return false;
}

// Função para buscar ultimo Nó dentro de uma Lista Ligada Simples
bool BuscaUltimoElementoListaSimples(ListaSimples *inicio, NoListaSimples **pos, NoListaSimples **ant)
{
    *pos = inicio;
    *ant = NULL;

    if(ListaSimplesVazia(inicio))
        return false;

    while (((*pos)->proximo != NULL)){
        (*ant) = (*pos);
        (*pos) = (*pos)->proximo;
    }

    return true;
}

// Função para criar um novo Nó na Lista Ligada
NoListaSimples *CriaNoSimples(int elem)
{
    NoListaSimples *novo = (NoListaSimples*)(calloc(1, sizeof(NoListaSimples)));
    novo->valor = elem;
    novo->proximo = NULL;

    return novo;
}

// Função para inserir um novo Nó depois do último Nó presente na Lista Ligada
void InserirElementoNoFimLista(ListaSimples **inicio, int valor)
{
    NoListaSimples *novo = CriaNoSimples(valor);
    NoListaSimples *pos, *ant;

    if (BuscaUltimoElementoListaSimples(*inicio, &pos, &ant))
        pos->proximo = novo;
    else
        *inicio = novo;
}

// Função para remover primeiro Nó na Lista Ligada
bool RemoveInicioListaSimples(ListaSimples **inicio, int *elem)
{
    NoListaSimples *aux = *inicio;

    if (!ListaSimplesVazia(*inicio))
    {
        (*inicio) = (*inicio)->proximo;
        free(aux);
        return true;
    }

    return false;
}

// Função para esvaziar a Lista Ligada
void DestroiListaLigada(ListaSimples **inicio)
{
    int elem;
    
    while (!ListaSimplesVazia(*inicio))
    {
        RemoveInicioListaSimples(inicio, &elem);
    }
}

// Função para desalocar a struct Imagem da memória
void DestroiImagem(Imagem **img)
{
    if ((*img) != NULL)
    {
        for (int y = 0; y < (*img)->altura; y++)
            free((*img)->Posicao[y]);
        free((*img)->Posicao);
        free((*img));
        (*img) = NULL;
    }
}

// Função para pegar um Nó localizado no indice posicional na Lista Ligada
NoListaSimples *ObterNo(NoListaSimples *noAtual, int indice)
{
    NoListaSimples *noAux = noAtual;

    for (int i = 0; i < indice && noAux != NULL; i++)
    {
        noAux = noAux->proximo;
    }
    return noAux;
}

Pixel *ObterAdjacentes(Pixel P)
{
    Pixel *vizinhos = (Pixel*)(calloc(8, sizeof(Pixel)));
    
    // vizinhos em relação ao pixel central
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) 
    {
        int novoX = P.x + dx[i];
        int novoY = P.y + dy[i];

        vizinhos[i].x = novoX;
        vizinhos[i].y = novoY;
    }

    return vizinhos;
}

// Função que realiza a operação de dilatação na matriz de números binários
// Essa função recebe o ponteiro para o nó atual na lista, total de linhas 'n'
// e total de colunas 'm' da matriz
void Dilatacao(NoListaSimples **noAtual, int n, int m)
{
    int i, j;
    int valor_referencia, valor;
    NoListaSimples *temp = (NoListaSimples *)calloc(n * m, sizeof(NoListaSimples));

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            valor_referencia = 0;
            Pixel P;
            P.x = i;
            P.y = j;
            Pixel *C = ObterAdjacentes(P);

            for (int k = 0; k < 8; k++)
            {
                if ((C[k].x >= 0) && (C[k].y >= 0) && (C[k].x < n) && (C[k].y < m)) // verifica se é uma posição 2D (i,j) válida no domínio da matriz
                {
                    // Obtenha valor de nó CANDIDATO usando a função ObterNo que recebe o noAtual e indice (i*m + j)
                    NoListaSimples *noCandidato = ObterNo(*noAtual, C[k].x * m + C[k].y);
                    valor = noCandidato->valor;

                    if (valor > valor_referencia) // atualize max se o valor for menor que min
                    {
                        valor_referencia = valor;
                    }
                }
            }

            // atribua valor_referencia para o membro valor do No no array temp.
            // Use indexação no array temp a partir da fórmula (i*m + j) do indice posicional que converte 
            // os indices (i,j) da matriz 2D para um indice da posição do nó correspondente na Lista Ligada.

           temp[i*m + j].valor = valor_referencia;
            
        }
    }

    // Copia os valores erodidos de volta para a lista original
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            // 1 passo) Acesse um nó da lista com ObterNo usando o noAtual e índice considerando as coordenadas (i,j) e dimensão (m) matriz 2D
            // 2 passo) Acesse o valor desse nó obtido
            // 3 passe) Altere esse valor acessado com o valor do nó correspondente no array temp.
            // Lembre que o array temp possui os valores dilatados nesse ponto da função,
            // então apenas acesse o nó no array temp usando o mesmo indice posicional usado no
            // passo 1.
            NoListaSimples *noAux = ObterNo(*noAtual, i*m + j);
            noAux->valor = temp[i*m + j].valor;

        }
    }

    free(temp);
}

void Erosao(NoListaSimples **noAtual, int n, int m)
{
    int i, j;
    int valor_referencia, valor;
    NoListaSimples *temp = (NoListaSimples *)calloc(n * m, sizeof(NoListaSimples));

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            valor_referencia = 1;
            Pixel P;
            P.x = i;
            P.y = j;
            Pixel *C = ObterAdjacentes(P);

            for (int k = 0; k < 8; k++)
            {
                if ((C[k].x >= 0) && (C[k].y >= 0) && (C[k].x < n) && (C[k].y < m)) // verifica se é uma posição 2D (i,j) válida no domínio da matriz
                {
                    // Obtenha valor de nó CANDIDATO usando a função ObterNo que recebe o noAtual e indice (i*m + j)
                    NoListaSimples *noAux = ObterNo(*noAtual, C[k].x * m + C[k].y);
                    valor = noAux->valor;

                    if (valor < valor_referencia) // atualize min se o valor for menor que min
                    {
                        valor_referencia = valor;
                    }
                }
            }

            // atribua valor_referencia para o membro valor do No no array temp.
            // Use indexação no array temp a partir da fórmula (i*m + j) do indice posicional que converte 
            // os indices (i,j) da matriz 2D para um indice da posição do nó correspondente na Lista Ligada.

            temp[i*m + j].valor = valor_referencia;
        }
    }

    // Copia os valores erodidos de volta para a lista original
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            // 1 passo) Acesse um nó da lista com ObterNo usando o noAtual e índice considerando as coordenadas (i,j) e dimensão (m) matriz 2D
            // 2 passo) Acesse o valor desse nó obtido
            // 3 passe) Altere esse valor acessado com o valor do nó correspondente no array temp.
            // Lembre que o array temp possui os valores dilatados nesse ponto da função,
            // então apenas acesse o nó no array temp usando o mesmo indice posicional usado no
            // passo 1.
            NoListaSimples *noAux = ObterNo(*noAtual, i*m + j);
            noAux->valor = temp[i*m + j].valor;

        }
    }

    free(temp);
}

void Abertura(NoListaSimples **noAtual, int n, int m)
{
    /*
    A operação de abertura é realizada primeiro aplicando a erosão, seguida
    pela dilatação. A abertura suaviza o contorno de uma imagem, quebra
    estreitamentos e elimina projeções finas.
    */
   Erosao(noAtual, n, m);
   Dilatacao(noAtual, n, m);
}

void Fechamento(NoListaSimples **noAtual, int n, int m)
{
    /*
    A operação de fechamento é o inverso da abertura. Primeiro, aplica-se a
    dilatação e, em seguida, a erosão. O fechamento também suaviza o contorno
    de uma imagem, mas, ao contrário da abertura, o fechamento geralmente
    funde estreitamentos e preenche buracos (valores '0s' com valores '1s' adjacentes) pequenos.
    */
   Dilatacao(noAtual, n, m);
   Erosao(noAtual, n, m);
}

// essa função mostra a Lista Ligada em um formato mais legível de matrizes numéricas
void VisualizarListaComoMatriz(NoListaSimples *cabeca, int linhas, int colunas)
{
    NoListaSimples *noAtual = cabeca;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if (noAtual != NULL)
            {
                printf("%d ", noAtual->valor);
                char caractere_valor_atual = noAtual->valor + '0';
                if (arquivo_saida_ponteiro != NULL)
                {
                    fprintf(arquivo_saida_ponteiro, "%c ", caractere_valor_atual);
                }

                // sprintf(str, "%d", num); // convert multiple digit integer to string
                noAtual = noAtual->proximo;
            }
        }
        printf("\n");
        if (arquivo_saida_ponteiro != NULL)
        {
            fprintf(arquivo_saida_ponteiro, "\n");
        }
    }
}

// Função para preencher uma Lista Ligada com os elementos obtidos de cada posição da Imagem
void PreencherListaLigadaComImagemBinaria(Imagem *imagem_binaria)
{
    int ncolunas = imagem_binaria->altura;
    int nlinhas = imagem_binaria->largura;
    for (int i = 0; i < ncolunas; i++)
    {
        for (int j = 0; j < nlinhas; j++)
        {
            InserirElementoNoFimLista(&noAtual, imagem_binaria->Posicao[i][j]);
        }
    }
}

// Função para exibir todos os resultados do programa no console
void ExibirResultados(Imagem *imagem_binaria, int NUM_COLUNAS, int NUM_LINHAS)
{
    printf("Dilation\n");
    if (arquivo_saida_ponteiro != NULL)
    {
        fprintf(arquivo_saida_ponteiro, "%s\n", "Dilation");
    }
    Dilatacao(&noAtual, NUM_COLUNAS, NUM_LINHAS);
    VisualizarListaComoMatriz(noAtual, NUM_LINHAS, NUM_COLUNAS);
    DestroiListaLigada(&noAtual);
    PreencherListaLigadaComImagemBinaria(imagem_binaria);

    printf("Erosion\n");
    if (arquivo_saida_ponteiro != NULL)
    {
        fprintf(arquivo_saida_ponteiro, "%s\n", "Erosion");
    }
    Erosao(&noAtual, NUM_COLUNAS, NUM_LINHAS);
    VisualizarListaComoMatriz(noAtual, NUM_LINHAS, NUM_COLUNAS);
    DestroiListaLigada(&noAtual);
    PreencherListaLigadaComImagemBinaria(imagem_binaria);

    printf("Opening\n");
    if (arquivo_saida_ponteiro != NULL)
    {
        fprintf(arquivo_saida_ponteiro, "%s\n", "Opening");
    }
    Abertura(&noAtual, NUM_COLUNAS, NUM_LINHAS);
    VisualizarListaComoMatriz(noAtual, NUM_LINHAS, NUM_COLUNAS);
    DestroiListaLigada(&noAtual);
    PreencherListaLigadaComImagemBinaria(imagem_binaria);

    printf("Closing\n");
    if (arquivo_saida_ponteiro != NULL)
    {
        fprintf(arquivo_saida_ponteiro, "%s\n", "Closing");
    }
    Fechamento(&noAtual, NUM_COLUNAS, NUM_LINHAS);
    VisualizarListaComoMatriz(noAtual, NUM_LINHAS, NUM_COLUNAS);
    DestroiListaLigada(&noAtual);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        Erro(
            "%s <arquivo texto de entrada> [<arquivo texto de saida>]\n"
            "Por exemplo, ./main ./arq1.in ./arq1.out\n",
            "main", argv[0]);
    }
    char *arquivo_saida = argv[2];
    if (arquivo_saida != NULL)
    {
        arquivo_saida_ponteiro = fopen(argv[2], "w");
        if (arquivo_saida_ponteiro == NULL)
        {
            printf("Falha na abertura do arquivo");
            return -1;
        }
    }

    Imagem *imagem_binaria = NULL;

    imagem_binaria = LeImagem(argv[1]);

    int NUM_LINHAS = imagem_binaria->largura;
    int NUM_COLUNAS = imagem_binaria->altura;

    PreencherListaLigadaComImagemBinaria(imagem_binaria);

    ExibirResultados(imagem_binaria, NUM_COLUNAS, NUM_LINHAS);

    DestroiImagem(&imagem_binaria);
    if (arquivo_saida_ponteiro != NULL)
    {
        fclose(arquivo_saida_ponteiro);
    }

    return 0;
}
