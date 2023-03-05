//
// Created by Alvaro on 05/03/2023.
//

#ifndef PARTE2_TRABALHO_NO_FOLHA_H
#define PARTE2_TRABALHO_NO_FOLHA_H


#include <stdio.h>

#include "funcionario.h"

typedef struct NoFolha {
    // m eh o numero total de registros armazenados no noh
    int m;
    // ponteiro para o noh pai
    int pont_pai;
    // ponteiro para o proximo noh folha
    int pont_prox;
    // essa lista tera sempre m func
    TFunc **func;
} TNoFolha;

// Imprime noh folha
void imprime_no_folha(int d, TNoFolha *no);

// Cria noh folha. Lembrar de usar libera_no_folha(no)
TNoFolha *no_folha(int d, int m, int pont_pai, int pont_prox);

// Cria noh folha. Lembrar de usar libera_no_folha(no)
TNoFolha *no_folha_vazio(int d);

// Cria noh folha com dados. Lembrar de usar libera_no_folha(no)
TNoFolha *cria_no_folha(int d, int pont_pai, int pont_prox, int m, ...);

// Grava uma pagina (noh) no arquivo em disco
void salva_no_folha(int d, TNoFolha *no, FILE *out);

// Le uma pagina (noh) do disco
TNoFolha *le_no_folha(int d, FILE *in);

// Compara dois nohs folhas
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_no_folha(int d, TNoFolha *n1, TNoFolha *n2);

// Retorna tamanho do noh folha em bytes
int tamanho_no_folha(int d);

// Libera noh folha e func
void libera_no_folha(int d, TNoFolha *no);

#endif //PARTE2_TRABALHO_NO_FOLHA_H
