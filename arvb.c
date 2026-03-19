#include "arvb.h"

#include <stdlib.h>
#include "arvb.h"

Btree criaPagina()
{
    No *novo;

    novo = (No *) malloc(sizeof(struct No));

    if (novo == NULL)
    {
        printf("Erro ao alocar memória!\n");
        return NULL;
    }

    novo->numchaves = 0;
    novo->folha = 1;

    for (int i = 0; i < ORDEM - 1; i++)
    {
        novo->chave[i] = 0;
        novo->posicao[i] = 0;
    }

    for (int i = 0; i < ORDEM; i++)
    {
        novo->filhos[i] = NULL;
    }

    return novo;
}

Btree insereArvoreB(Btree a, int chave)
{
    
}

int buscaArvoreB(Btree a, int chave)
{
    
}

void destroiArvoreB(Btree a)
{
    
}