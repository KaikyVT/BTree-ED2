#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvb.h"


Btree  g_arvore   = NULL;
char   g_arqDados[256];
char   g_arqArvore[256];

int leRegistro(long offset, char *nome, char *telefone)
{
    FILE *f = fopen(g_arqDados, "r");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo de dados.\n");
        return 0;
    }

    fseek(f, offset, SEEK_SET);   

    char linha[256];
    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        return 0;
    }
    fclose(f);

    linha[strcspn(linha, "\n")] = '\0';

    char *p1 = strchr(linha, '/');
    if (!p1) return 0;
    char *p2 = strchr(p1 + 1, '/');
    if (!p2) return 0;

    *p1 = '\0';
    *p2 = '\0';
    strncpy(nome,     p1 + 1, 127);
    strncpy(telefone, p2 + 1, 31);
    return 1;
}

int qtdRegistros()
{
    FILE *f = fopen(g_arqDados, "r");
    if (!f) return 0;
    int n = 0;
    fscanf(f, "%d\n", &n);
    fclose(f);
    return n;
}

void atualizaQtd(int novaQtd)
{
    FILE *f = fopen(g_arqDados, "r");
    if (!f) return;

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);

    char *buf = (char *)malloc(tam + 1);
    fread(buf, 1, tam, f);
    buf[tam] = '\0';
    fclose(f);

    char *resto = strchr(buf, '\n');
    if (!resto) { free(buf); return; }
    resto++; 

    f = fopen(g_arqDados, "w");
    if (!f) { free(buf); return; }
    fprintf(f, "%d\n%s", novaQtd, resto);
    fclose(f);
    free(buf);
}

void cadastro()
{
    int  matricula;
    char nome[128];
    char telefone[32];

    printf("\n=== CADASTRAR ===\n");
    printf("Matrícula : ");
    scanf("%d", &matricula);
    getchar(); 

    printf("Nome      : ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Telefone  : ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    if (buscaArvoreB(g_arvore, matricula) != -1) {
        printf("\nMatrícula já cadastrada!\n");
        return;
    }

    FILE *f = fopen(g_arqDados, "a");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo de dados para escrita.\n");
        return;
    }

    long offset = ftell(f);
    fprintf(f, "%d/%s/%s\n", matricula, nome, telefone);
    fclose(f);

    g_arvore = insereArvoreB(g_arvore, matricula, offset);

    atualizaQtd(qtdRegistros() + 1);

    printf("\nAluno cadastrado com sucesso!\n");
}

void pesquisa()
{
    int matricula;
    printf("\n=== PESQUISAR ===\n");
    printf("Matrícula : ");
    scanf("%d", &matricula);
    getchar();

    long offset = buscaArvoreB(g_arvore, matricula);
    if (offset == -1) {
        printf("\nMatrícula %d não encontrada.\n", matricula);
        return;
    }

    char nome[128]     = {0};
    char telefone[32]  = {0};

    if (leRegistro(offset, nome, telefone)) {
        printf("\n--- Registro encontrado ---\n");
        printf("Matrícula : %d\n", matricula);
        printf("Nome      : %s\n", nome);
        printf("Telefone  : %s\n", telefone);
    } else {
        printf("Erro ao ler registro no arquivo.\n");
    }
}

void gravacao()
{
    FILE *f = fopen(g_arqArvore, "w");
    if (f == NULL) {
        fprintf(stderr, "Erro ao criar arquivo da árvore '%s'.\n", g_arqArvore);
        return;
    }

    fprintf(f, "RAIZ: %p\n", (void *)g_arvore);

    gravaArvoreB(g_arvore, f);

    fclose(f);
    printf("\nÁrvore gravada em '%s' com sucesso!\n", g_arqArvore);
}

void exibeMenu()
{
    printf("\n╔══════════════════════════════╗\n");
    printf("║   Sistema de Cadastro Alunos ║\n");
    printf("╠══════════════════════════════╣\n");
    printf("║  [1] Cadastrar               ║\n");
    printf("║  [2] Pesquisar               ║\n");
    printf("║  [3] Gravar árvore           ║\n");
    printf("║  [4] Sair                    ║\n");
    printf("╚══════════════════════════════╝\n");
    printf("> ");
}

int main()
{
    strncpy(g_arqDados,  "dados.txt",  255);
    strncpy(g_arqArvore, "index.txt", 255);

    printf("Carregando registros de '%s'...\n", g_arqDados);
    g_arvore = carregaArvore(g_arqDados);
    printf("Árvore B carregada com sucesso!\n");

    int opcao;
    do {
        exibeMenu();
        if (scanf("%d", &opcao) != 1) {
            getchar();
            opcao = 0;
        }
        getchar();

        switch (opcao) {
            case 1: cadastro(); break;
            case 2: pesquisa(); break;
            case 3: gravacao(); break;
            case 4:
                printf("\nLiberando memória e encerrando...\n");
                destroiArvoreB(g_arvore);
                g_arvore = NULL;
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}