#include "AVL.h"

/*----------- Fun�oes Privadas -----------------------------------------*/

char RemoveNoGrau0ou1(AVL **ab, char *mais_baixa);
void SubstituiRemoveMenorSucessor(AVL **ab, AVL **maisesq, char *mais_baixa);
void CalculaFrequencia(AVL *ab, int **f);
void RotacaoSimplesEsquerda(AVL **ab);
void RotacaoDuplaEsquerda(AVL **ab);
void RotacaoSimplesDireita(AVL **ab);
void RotacaoDuplaDireita(AVL **ab);
void AjusteBalancoDuplaInsercao(NoAVL *A, NoAVL *B, NoAVL *C);
void AjusteBalancoSimplesInsercao(NoAVL *A, NoAVL *B);
void AjusteBalancoDuplaRemocao(NoAVL *A, NoAVL *B, NoAVL *C);
void AjusteBalancoRotacaoDireitaSimplesRemocao(NoAVL *A, NoAVL *B, char *mais_baixa);
void AjusteBalancoRotacaoEsquerdaSimplesRemocao(NoAVL *A, NoAVL *B, char *mais_baixa);
void TrataAumentoArvoreEsquerda(AVL **ab, char *mais_alta);
void TrataAumentoArvoreDireita(AVL **ab, char *mais_alta);
void TrataReducaoArvoreEsquerda(AVL **ab, char *mais_baixa);
void TrataReducaoArvoreDireita(AVL **ab, char *mais_baixa);
void RemoveDeFato(AVL **ab, char *mais_baixa);

/* Rotacao Simples a Esquerda: O filho a direita da raiz passa a ser o
   seu neto a esquerda do seu filho a direita, a raiz passa a ser
   filha a esquerda de seu filho a direita, e o filho a direita vira a
   nova raiz. */

void RotacaoSimplesEsquerda(AVL **ab)
{
  AVL *filho_dir = (*ab)->dir; /* salva filho a direita */

  (*ab)->dir = filho_dir->esq;
  filho_dir->esq = (*ab);
  (*ab) = filho_dir;
}

/* Rotacao dupla a esquerda equivale a rotacao simples a direita da
   subarvore direita seguida de rotacao simples a esquerda */

void RotacaoDuplaEsquerda(AVL **ab)
{
  RotacaoSimplesDireita(&(*ab)->dir);
  RotacaoSimplesEsquerda(ab);
}

/* Rotacao Simples a Direita: O filho a esquerda da raiz passa a ser o
   seu neto a direita do seu filho a esquerda, a raiz passa a ser
   filha a direita de seu filho a esquerda, e o filho a esquerda vira
   a nova raiz. */

void RotacaoSimplesDireita(AVL **ab)
{
  AVL *filho_esq = (*ab)->esq; /* salva filho a esquerda */

  (*ab)->esq = filho_esq->dir;
  filho_esq->dir = (*ab);
  (*ab) = filho_esq;
}

/* Rotacao dupla a direita equivale a rotacao simples a esquerda da
   subarvore esquerda seguida de rotacao simples a direita */

void RotacaoDuplaDireita(AVL **ab)
{
  RotacaoSimplesEsquerda(&(*ab)->esq);
  RotacaoSimplesDireita(ab);
}

/* Trata remo�ao de no folha ou com um unicio filho, retornando 1. Se
   este nao for o caso, retorna 0. */

char RemoveNoGrau0ou1(AVL **ab, char *mais_baixa)
{
  AVL *aux;

  if ((*ab)->esq == NULL)
  { /* (*ab) eh folha ou tem unico filho */
    aux = (*ab);
    (*ab) = (*ab)->dir;
    free(aux);
  }
  else
  {
    if ((*ab)->dir == NULL)
    { /* (*ab) eh folha ou tem unico filho */
      aux = (*ab);
      (*ab) = (*ab)->esq;
      free(aux);
    }
    else
    {
      return (0);
    }
  }
  *mais_baixa = 1;
  return (1);
}

/* Substitui o valor de um no, cujo valor esta sendo removido, pelo
   valor do menor sucessor deste no, e remove o no do menor
   sucessor. Auxilia RemoveValor. */

void SubstituiRemoveMenorSucessor(AVL **ab, AVL **maisesq, char *mais_baixa)
{
  if ((*maisesq)->esq == NULL)
  {
    (*ab)->info = (*maisesq)->info;        /* substitui valor */
    RemoveNoGrau0ou1(maisesq, mais_baixa); /* remove no */
  }
  else
  { /* busca sucessor */
    SubstituiRemoveMenorSucessor(ab, &((*maisesq)->esq), mais_baixa);
    if (*mais_baixa)
      TrataReducaoArvoreEsquerda(maisesq, mais_baixa); /* corrige balan�o
                da arvore do
                pai do
                sucessor */
  }
}

/* Acumula a frequencia dos valores da arvore ab em vetor f para
   auxiliar a fun�ao Frequencia. */

void CalculaFrequencia(AVL *ab, int **f)
{
  if (ab != NULL)
  {
    (*f)[ab->info] += 1;
    CalculaFrequencia(ab->esq, f);
    CalculaFrequencia(ab->dir, f);
  }
}

void AjusteBalancoDuplaInsercao(NoAVL *A, NoAVL *B, NoAVL *C)
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
    A->bal = 0;
    B->bal = -1;
    break;
  }
  C->bal = 0;
}

void AjusteBalancoSimplesInsercao(NoAVL *A, NoAVL *B)
{
  A->bal = B->bal = 0;
}

void AjusteBalancoRotacaoDireitaDuplaRemocao(NoAVL *A, NoAVL *B, NoAVL *C)
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
    A->bal = 0;
    B->bal = -1;
    break;
  }
  C->bal = 0;
}

void AjusteBalancoRotacaoEsquerdaDuplaRemocao(NoAVL *A, NoAVL *B, NoAVL *C)
{
  switch (C->bal)
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
    B->bal = 0;
    break;
  }
  C->bal = 0;
}

void AjusteBalancoRotacaoDireitaSimplesRemocao(NoAVL *A, NoAVL *B, char *mais_baixa)
{
  if (B->bal == -1)
  {
    A->bal = 0;
    B->bal = 0;
    *mais_baixa = 1;
  }
  else
  { /* B->bal == 0 */
    A->bal = -1;
    B->bal = 1;
    *mais_baixa = 0;
  }
}

void AjusteBalancoRotacaoEsquerdaSimplesRemocao(NoAVL *A, NoAVL *B, char *mais_baixa)
{
  if (B->bal == 1)
  {
    A->bal = 0;
    B->bal = 0;
    *mais_baixa = 1;
  }
  else
  { /* B->bal == 0 */
    A->bal = 1;
    B->bal = -1;
    *mais_baixa = 0;
  }
}

void TrataAumentoArvoreEsquerda(AVL **ab, char *mais_alta)
{
  NoAVL *A, *B, *C;

  A = (*ab);
  B = (*ab)->esq;
  C = B->dir;

  switch (A->bal)
  {
  case -1: /* arvore perdeu o balanceamento */
    if (B->bal == -1)
    { /* rebalancear com rotacao
simples a direita, pois o novo
no foi inserido a esquerda do
filho esquerdo */
      RotacaoSimplesDireita(ab);
      AjusteBalancoSimplesInsercao(A, B);
    }
    else
    { /* entao B->bal == +1, rebalancear com rotacao dupla
   a direita, pois o novo no foi inserido a direita
   do filho esquerdo */
      RotacaoDuplaDireita(ab);
      AjusteBalancoDuplaInsercao(A, B, C);
    }
    (*mais_alta) = 0;
    break;
  case 0:
    (*ab)->bal = -1;
    (*mais_alta) = 1;

    break;
  case 1:
    (*ab)->bal = 0;
    (*mais_alta) = 0;
    break;
  }
}

void TrataAumentoArvoreDireita(AVL **ab, char *mais_alta)
{
  NoAVL *A, *B, *C;

  A = (*ab);
  B = (*ab)->dir;
  C = B->esq;

  switch (A->bal)
  {
  case 1: /* arvore perdeu o balanceamento */
    if (B->bal == 1)
    { /* rebalancear com rotacao
simples a esquerda, pois o
novo no foi inserido a direira
do filho direito */
      RotacaoSimplesEsquerda(ab);
      AjusteBalancoSimplesInsercao(A, B);
    }
    else
    { /* entao B->bal == -1, rebalancear com
   rotacao dupla a esquerda, pois o novo no foi
   inserido a esquerda do filho direito */
      RotacaoDuplaEsquerda(ab);
      AjusteBalancoDuplaInsercao(B, A, C); /* troca A e B para usar a
                mesma funcao */
    }
    *mais_alta = 0;
    break;
  case 0:
    A->bal = 1;
    (*mais_alta) = 1;
    break;
  case -1:
    A->bal = 0;
    (*mais_alta) = 0;
    break;
  }
}

void TrataReducaoArvoreEsquerda(AVL **ab, char *mais_baixa)
{
  NoAVL *A, *B, *C;

  A = (*ab);
  B = (*ab)->dir;

  switch (A->bal)
  {
  case 1: /* arvore perdeu o balanceamento */
    if (B->bal >= 0)
    { /* rebalancear com rotacao
simples a esquerda */
      RotacaoSimplesEsquerda(ab);
      AjusteBalancoRotacaoEsquerdaSimplesRemocao(A, B, mais_baixa);
      printf("%d\n", *mais_baixa);
    }
    else
    { /* entao B->bal == -1, rebalancear com rotacao dupla a
   esquerda */
      C = B->esq;
      RotacaoDuplaEsquerda(ab);
      AjusteBalancoRotacaoEsquerdaDuplaRemocao(A, B, C);
      (*mais_baixa) = 1;
    }
    break;
  case 0:
    A->bal = 1;
    (*mais_baixa) = 0;
    break;
  case -1:
    A->bal = 0;
    (*mais_baixa) = 1;
    break;
  }
}

void TrataReducaoArvoreDireita(AVL **ab, char *mais_baixa)
{
  NoAVL *A, *B, *C;

  A = (*ab);
  B = (*ab)->esq;

  switch (A->bal)
  {
  case -1: /* arvore perdeu o balanceamento */
    if (B->bal <= 0)
    { /* rebalancear com rotacao
simples a direita */
      RotacaoSimplesDireita(ab);
      AjusteBalancoRotacaoDireitaSimplesRemocao(A, B, mais_baixa);
    }
    else
    { /* B->bal == 1, rebalancear com rotacao dupla a direita */
      C = B->dir;
      RotacaoDuplaDireita(ab);
      AjusteBalancoRotacaoDireitaDuplaRemocao(A, B, C);
      (*mais_baixa) = 1;
    }
    break;
  case 0:
    (*ab)->bal = -1;
    (*mais_baixa) = 0;
    break;
  case 1:
    (*ab)->bal = 0;
    (*mais_baixa) = 1;
    break;
  }
}

/*------------------- Fun�oes Publicas ---------------------------------*/

/* Aloca e preenche um novo n� folha (insercao eh sempre na filha) */

NoAVL *CriaNovoNo(int info)
{

  NoAVL *p = (NoAVL *)calloc(1, sizeof(NoAVL));
  p->esq = NULL;
  p->dir = NULL;
  p->bal = 0;
  p->info = info;

  return (p);

} /* NovoNo */

void InsereValor(AVL **ab, int valor, char *mais_alta)
{

  if ((*ab) == NULL)
  {
    (*ab) = CriaNovoNo(valor);
    *mais_alta = 1;
  }
  else
  {
    if ((*ab)->info <= valor)
    { /* insercao a direita */
      InsereValor(&((*ab)->dir), valor, mais_alta);
      if ((*mais_alta) == 1)
      { /* subarvore direita voltou mais alta */
        TrataAumentoArvoreDireita(ab, mais_alta);
      }
    }
    else
    { /* insercao a esquerda */
      InsereValor(&((*ab)->esq), valor, mais_alta);
      if ((*mais_alta) == 1)
      { /* subarvore esquerda voltou mais alta */
        TrataAumentoArvoreEsquerda(ab, mais_alta);
      }
    }
  }
}

void RemoveDeFato(AVL **ab, char *mais_baixa)
{
  int bal;
  if (RemoveNoGrau0ou1(ab, mais_baixa) == 0)
  {                   /* Remove no de grau 2 */
    bal = (*ab)->bal; /* salva balanco */
    SubstituiRemoveMenorSucessor(ab, &((*ab)->dir), mais_baixa);
    (*ab)->bal = bal; /* recupera balanco */
    if (*mais_baixa)
      TrataReducaoArvoreDireita(ab, mais_baixa); /* corrige balan�o
                da arvore direita
                apos substituicao
                pelo sucessor */
  }
}

void RemoveValor(AVL **ab, int valor, char *mais_baixa)
{
  if ((*ab) != NULL)
  {
    if ((*ab)->info != valor)
    {
      if ((*ab)->info < valor)
      {
        RemoveValor(&((*ab)->dir), valor, mais_baixa);
        if ((*mais_baixa) == 1)
          TrataReducaoArvoreDireita(ab, mais_baixa);
      }
      else
      {
        RemoveValor(&((*ab)->esq), valor, mais_baixa);
        if ((*mais_baixa) == 1)
          TrataReducaoArvoreEsquerda(ab, mais_baixa);
      }
    }
    else
    { /* (*ab)->info == valor */
      RemoveDeFato(ab, mais_baixa);
    }
  }
}

void VisitaPreOrdem(AVL *ab, char **s)
{
  if (ab != NULL)
  {
    sprintf(*s, "%d ", ab->info);
    (*s) = (*s) + strlen(*s);
    VisitaPreOrdem(ab->esq, s);
    VisitaPreOrdem(ab->dir, s);
  }
}

void VisitaInOrdem(AVL *ab, char **s)
{
  if (ab != NULL)
  {
    VisitaInOrdem(ab->esq, s);
    sprintf(*s, "%d ", ab->info);
    (*s) = (*s) + strlen(*s);
    VisitaInOrdem(ab->dir, s);
  }
}

void VisitaPosOrdem(AVL *ab, char **s)
{
  if (ab != NULL)
  {
    VisitaPosOrdem(ab->esq, s);
    VisitaPosOrdem(ab->dir, s);
    sprintf(*s, "%d ", ab->info);
    (*s) = (*s) + strlen(*s);
  }
}

void DestroiAVL(AVL **ab)
{
  char mais_baixa;

  while ((*ab) != NULL)
    RemoveValor(ab, (*ab)->info, &mais_baixa);
}

int NumerodeNos(AVL *ab)
{
  if (ab != NULL)
    return (1 + NumerodeNos(ab->esq) + NumerodeNos(ab->dir));
  else
    return (0);
}

int Altura(AVL *ab)
{
  if (ab != NULL)
    return (1 + MAX(Altura(ab->esq), Altura(ab->dir)));
  else
    return (0);
}

void MaioresValores(AVL *ab, char **s, int *k)
{
  if ((ab != NULL) && ((*k) > 0))
  {
    MaioresValores(ab->dir, s, k);
    if ((*k) > 0)
    {
      sprintf(*s, "%d ", ab->info);
      (*s) = (*s) + strlen(*s);
      (*k)--;
    }
    MaioresValores(ab->esq, s, k);
  }
}

void MenoresValores(AVL *ab, char **s, int *k)
{
  if ((ab != NULL) && ((*k) > 0))
  {
    MenoresValores(ab->esq, s, k);
    if ((*k) > 0)
    {
      sprintf(*s, "%d ", ab->info);
      (*s) = (*s) + strlen(*s);
      (*k)--;
    }
    MenoresValores(ab->dir, s, k);
  }
}

void Frequencia(AVL *ab, int **f, int *n)
{
  int maior, i;
  char *s, saida[TAM_MAX];

  if (ab != NULL)
  {
    s = saida;
    i = 1;
    MaioresValores(ab, &s, &i);
    maior = atoi(saida);
    *n = maior + 1;
    if ((*f) != NULL)
      free((*f));
    (*f) = (int *)calloc(*n, sizeof(int));
    CalculaFrequencia(ab, f);
  }
  else
    *n = 0;
}

void EliminaRepetidos(AVL **ab)
{
  int i, *f = NULL, n;
  char mais_baixa;

  if ((*ab) != NULL)
  {
    Frequencia((*ab), &f, &n);
    for (i = 0; i < n; i++)
      while (f[i] > 1)
      {
        RemoveValor(ab, i, &mais_baixa);
        f[i] = f[i] - 1;
      }
    free(f);
  }
}

void ImprimeAVL(AVL *ab, int h, int altura)
{
  int i;

  if (ab != NULL)
  {
    ImprimeAVL(ab->dir, h + 1, altura);
    for (i = 0; i < h; i++)
      printf("   ");
    if (ab->bal < 0)
      printf("%02d(%d)", ab->info, ab->bal);
    else
      printf("%02d( %d)", ab->info, ab->bal);
    for (i = 0; i < altura - h; i++)
      printf("---");
    printf("\n");
    ImprimeAVL(ab->esq, h + 1, altura);
  }
}

char Equilibrada(AVL *ab, int *N)
{
  int Ne, Nd;

  if (ab == NULL)
  {
    *N = 0;
    return (1);
  }
  else
  {
    if (Equilibrada(ab->esq, &Ne) &&
        Equilibrada(ab->dir, &Nd) &&
        (abs(Ne - Nd) <= 1))
    {
      (*N) = Ne + Nd + 1;
      printf("info %d Ne %d Nd %d\n", ab->info, Ne, Nd);
      return (1);
    }
    else
    {
      return (0);
    }
  }
}
