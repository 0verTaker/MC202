#ifndef _AVL_H_
#define _AVL_H_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define MAX(a,b) ((a > b)?a:b)
#define TAM_MAX 100

/* Arvore Binaria de Busca de Altura Balanceada (tipo AVL) com valores
   menores na subarvore esquerda e maiores ou iguais na subarvore
   direita de cada no */

typedef struct NoAVL {  /* Registro que representa nó em árvore
			   binária */
  int info;
  int bal; /* 0: subarvores esquerda e direita de mesma altura, -1: esquerda mais alta, e +1: direita mais alta. */
  struct NoAVL *esq, *dir;
} NoAVL, AVL;

/* cria no folha, insercao eh sempre na folha */

NoAVL *CriaNovoNo(int info); 

/* Esta função deve inserir o valor da variável "valor" na árvore
   binária de busca representada pela variável "ab". Note que valores
   iguais devem ser inseridos sempre à direita. Apos insercao, na
   volta da recursao, verificar se a altura da subarvore aumentou
   (i.e. mais_alta = 1), e se com isso a arvore ficou
   desbalanceada. Caso afirmativo, balancear a arvore e voltar
   mais_alta para 0. */

void InsereValor(AVL **ab, int valor, char *mais_alta); 

/* Esta função deve percorrer a árvore ab em pré-ordem e gerar uma
   cadeia de caracteres s com os valores visitados em pré-ordem
   separados por um espaço em branco. */

void VisitaPreOrdem(AVL *ab, char **s); 

/* Esta função deve percorrer a árvore ab em in-ordem e gerar uma
   cadeia de caracteres s com os valores visitados em in-ordem
   separados por um espaço em branco. */

void VisitaInOrdem(AVL *ab, char **s); 

/* Esta função deve percorrer a árvore ab em pós-ordem e gerar uma
   cadeia de caracteres s com os valores visitados em pós-ordem
   separados por um espaço em branco. */

void VisitaPosOrdem(AVL *ab, char **s);  

/* Esta função deve remover o valor da variável "valor" da árvore
   binária de busca representada pela variável "ab". Note que no caso
   de remoção de nó de grau 2, seu valor deve ser substituído pelo
   valor do menor sucessor (no a esquerda com menor valor da subarvore
   direita), e o no sucessor deve ser removido. Isso porque a arvore
   resolve a igualdade inserindo o no na subarvore direita. Este
   sucessor nao tem filho a esquerda, mas pode ter filho a
   direita. Portanto, o no pai do sucessor deve apontar para o seu
   filho a direita antes da remocao do sucessor. Se a igualdade fosse
   resolvida com insercao na subarvore esquerda, deveriamos procurar o
   maior antecessor para substituicao. Neste caso, ele nao teria filho
   a direita, mas poderia ter filho a esquerda e o seu no pai deveria
   apontar para o filho a esquerda antes da remocao. Apos remocao, na
   volta da recursao, verificar se a altura da subarvore diminuiu
   (i.e. mais_baixa = 1), e se com isso a arvore ficou
   desbalanceada. Caso afirmativo, balancear a arvore e voltar
   mais_baixa para 0. */

void RemoveValor(AVL **ab, int valor, char *mais_baixa); 

/* Esta função deve liberar todo espaço de memória alocado para a
   árvore */

void DestroiAVL(AVL **ab); 

/* Esta função deve calcular e retornar o número de nós da árvore
   ab */

int  NumerodeNos(AVL *ab); 

/* Esta função deve calcular e retornar a altura da árvore ab */

int  Altura(AVL *ab); 

/* Esta função deve calcular os k maiores valores armazenados na
   árvore ab e retorná-los em ordem decrescente e separados por um
   espaço em branco na cadeia de caracteres s */

void MaioresValores(AVL *ab, char **s, int *k); 

/* Esta função deve calcular os k menores valores armazenados na
   árvore ab e retorná-los em ordem crescente e separados por um
   espaço em branco na cadeia de caracteres s */

void MenoresValores(AVL *ab, char **s, int *k); 

/* Esta função supõe que a árvore ab armazena apenas valores não
   negativos. Ela deve calcular o maior valor K armazenado na árvore e
   criar um vetor de freqüência f de tamanho n=K+1. Depois ela deve
   calcular e armazenar em f[i] o número de ocorrências do valor i na
   árvore ab. Por fim, ela deve retornar o vetor f e seu tamanho n. */

void Frequencia(AVL *ab, int **f, int *n); 

/* Esta função deve remover da árvore ab todos os valores repetidos
   deixando apenas uma ocorrência de cada valor. */

void EliminaRepetidos(AVL **ab); 

/* Imprime arvore binaria na tela onde h (altura do no) indica que ele
   sera precedido de h caracteres '-' antes de sua impressao. A funcao
   e chamada com h=0. */

void ImprimeAVL(AVL *ab, int h, int altura);

/* Arvore Equilibrada: aquela que os numeros de nos das subarvores
   direita e esquerda nao diferem mais do que 1 para qualquer no. */

char Equilibrada(AVL *ab, int *N); 

#endif









