#include <stdio.h>

#define ORDEM 4

typedef struct No {
    int numchaves;
    int chave[ORDEM-1];
    struct No *filhos[ORDEM];
    unsigned int posicao[ORDEM-1];
    int folha;
} No;

typedef No *Btree;

No criaPagina();
Btree insereArvoreB(Btree a, int chave);
int buscaArvoreB(Btree a, int chave);
void destroiArvoreB(Btree a);