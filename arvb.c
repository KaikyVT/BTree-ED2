#include "arvb.h"
#include <stdlib.h>
#include <string.h>

No *criaPagina()
{
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória!\n");
        return NULL;
    }
    novo->numchaves = 0;
    novo->folha     = 1;
    for (int i = 0; i < MAX_CHAVES; i++) {
        novo->chave[i]   = 0;
        novo->posicao[i] = -1;
    }
    for (int i = 0; i < ORDEM; i++) {
        novo->filhos[i] = NULL;
    }
    return novo;
}

void destroiArvoreB(Btree a)
{
    if (a == NULL) return;
    if (!a->folha) {
        for (int i = 0; i <= a->numchaves; i++) {
            destroiArvoreB(a->filhos[i]);
        }
    }
    free(a);
}


long buscaArvoreB(Btree a, int chave)
{
    if (a == NULL) return -1;

    int i = 0;
    while (i < a->numchaves && chave > a->chave[i])
        i++;

    if (i < a->numchaves && chave == a->chave[i])
        return a->posicao[i];

    if (a->folha) return -1;
    return buscaArvoreB(a->filhos[i], chave);
}

void splitFilho(No *pai, int idx)
{
    No *cheio  = pai->filhos[idx];
    No *novo   = criaPagina();
    int medio  = MAX_CHAVES / 2; 

    novo->folha     = cheio->folha;
    novo->numchaves = MAX_CHAVES - medio - 1;

    for (int j = 0; j < novo->numchaves; j++) {
        novo->chave[j]   = cheio->chave[medio + 1 + j];
        novo->posicao[j] = cheio->posicao[medio + 1 + j];
    }

    if (!cheio->folha) {
        for (int j = 0; j <= novo->numchaves; j++)
            novo->filhos[j] = cheio->filhos[medio + 1 + j];
    }

    cheio->numchaves = medio; 

    for (int j = pai->numchaves; j > idx; j--)
        pai->filhos[j + 1] = pai->filhos[j];

    pai->filhos[idx + 1] = novo;

    for (int j = pai->numchaves - 1; j >= idx; j--) {
        pai->chave[j + 1]   = pai->chave[j];
        pai->posicao[j + 1] = pai->posicao[j];
    }
    pai->chave[idx]   = cheio->chave[medio];
    pai->posicao[idx] = cheio->posicao[medio];
    pai->numchaves++;
}

void insereNaoCheio(No *a, int chave, long posicao)
{
    int i = a->numchaves - 1;

    if (a->folha) {
        while (i >= 0 && chave < a->chave[i]) {
            a->chave[i + 1]   = a->chave[i];
            a->posicao[i + 1] = a->posicao[i];
            i--;
        }
        a->chave[i + 1]   = chave;
        a->posicao[i + 1] = posicao;
        a->numchaves++;
    } else {
        while (i >= 0 && chave < a->chave[i])
            i--;
        i++; 

        if (a->filhos[i]->numchaves == MAX_CHAVES) {
            splitFilho(a, i);
            if (chave > a->chave[i])
                i++;
        }
        insereNaoCheio(a->filhos[i], chave, posicao);
    }
}

Btree insereArvoreB(Btree a, int chave, long posicao)
{
    if (a == NULL) {
        No *raiz = criaPagina();
        raiz->chave[0]   = chave;
        raiz->posicao[0] = posicao;
        raiz->numchaves  = 1;
        raiz->folha = 1;
        return raiz;
    }

    if (a->numchaves == MAX_CHAVES) {
        No *novaRaiz = criaPagina();
        novaRaiz->folha = 0;
        novaRaiz->filhos[0] = a;
        splitFilho(novaRaiz, 0);
        insereNaoCheio(novaRaiz, chave, posicao);
        return novaRaiz;
    }

    insereNaoCheio(a, chave, posicao);
    return a;
}

void gravaArvoreB(Btree a, FILE *f)
{
    if (a == NULL) return;

    No  *fila[4096];
    int  ini = 0, fim = 0;
    fila[fim++] = a;

    while (ini < fim) {
        No *atual = fila[ini++];

        fprintf(f, "NO| %p ", atual);
        for (int i = 0; i < atual->numchaves; i++) {
            fprintf(f, "chave=%d pos=%ld", atual->chave[i], atual->posicao[i]);
            if (i < atual->numchaves - 1) fprintf(f, " | ");
        }

        fprintf(f, " || filhos:");
        for (int i = 0; i <= atual->numchaves; i++) {
            if (atual->filhos[i])
                fprintf(f, " %p", (void *)atual->filhos[i]);
            else
                fprintf(f, " NULL");
        }
        fprintf(f, "\n");

        if (!atual->folha) {
            for (int i = 0; i <= atual->numchaves; i++) {
                if (atual->filhos[i])
                    fila[fim++] = atual->filhos[i];
            }
        }
    }
}

Btree carregaArvore(const char *arqDados)
{
    FILE *f = fopen(arqDados, "r");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo '%s'\n", arqDados);
        return NULL;
    }

    Btree raiz = NULL;
    char  linha[256];

    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        return NULL;
    }

    while (1) {
        long offset = ftell(f);          
        if (fgets(linha, sizeof(linha), f) == NULL) break;

        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) == 0) continue;

        char *sep = strchr(linha, '/');
        if (sep == NULL) continue;
        *sep = '\0';
        int matricula = atoi(linha);

        raiz = insereArvoreB(raiz, matricula, offset);
    }

    fclose(f);
    return raiz;
}