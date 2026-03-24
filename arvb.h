#define ARVB_H

#include <stdio.h>

#define ORDEM 4
#define MAX_CHAVES (ORDEM - 1)

typedef struct No {
    int        numchaves;
    int        chave[MAX_CHAVES];
    long       posicao[MAX_CHAVES];
    struct No *filhos[ORDEM];
    int        folha;
} No;

typedef No *Btree;

No    *criaPagina();
void   destroiArvoreB(Btree a);

Btree  insereArvoreB(Btree a, int chave, long posicao);

long   buscaArvoreB(Btree a, int chave);

void   gravaArvoreB(Btree a, FILE *f);

Btree  carregaArvore(const char *arqDados);