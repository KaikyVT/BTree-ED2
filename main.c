#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvb.h"



void cadastro();
void pesquisa();
void gravacao();
void printaMenu();
void pegaInfo();

void cadastro()
{
    printf("Cadastrado\n");
    printaMenu();
}
void pesquisa()
{
    printf("Pesquisado\n");
    printaMenu();
}
void gravacao()
{
    printf("Gravado\n");
    printaMenu();
}

void pegaInfo()
{
    int resposta;
    scanf("%d", &resposta);
    switch(resposta)
    {
        case 1: cadastro(); break;
        case 2: pesquisa(); break;
        case 3: gravacao(); break;
        case 4: return;
        default: printf("inválido... Digite outra resposta:\n"); pegaInfo();
    }
}

void printaMenu()
{
    system("clear");
    printf("Bem vindo ao sistema cadastral! O que deseja fazer?\n");
    printf("[1]- Cadastrar um novo aluno\n");
    printf("[2]- Pesquisar um aluno\n");
    printf("[3]- Gravar informações\n");
    printf("[4]- Sair\n");
    printf("> ");
    pegaInfo();
}




int main()
{
    printaMenu();
}
